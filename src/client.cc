//
// Created by Nishanth Prabhu on 06/04/19.
//

#include "client.h"
#include "rpc/server.h"

#define MESSAGE_LEN 256

static void init(void)
{
    if (sodium_init() != 0)
        abort();
    printf("Using libsodium %s\n", sodium_version_string());
}

/*
* get_hex() is a wrapper around sodium_bin2hex() which allocates
* temporary memory then immediately returns the result followed by \n
*/
string get_hex(const unsigned char *bin, const size_t bin_len)
{
    char   *hex;
    size_t  hex_size;
    string str_key;

    if (bin_len >= SIZE_MAX / 2)
    abort();

    hex_size = bin_len * 2 + 1;
    if ((hex = (char *)malloc(hex_size)) == NULL)
        abort();

    /* the library supplies a few utility functions like the one below */
    if (sodium_bin2hex(hex, hex_size, bin, bin_len) == NULL)
        abort();

    str_key = string(hex);
    
    free(hex);  
    
    return str_key;  
}

string getIPAddress(){
    string ipAddress="Unable to get IP Address";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                // Check if interface is en0 which is the wifi connection on the iPhone
                //if(strcmp(temp_addr->ifa_name, "en0")==0){
                    ipAddress=inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
                //}
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    // Free memory
    freeifaddrs(interfaces);
    return ipAddress;
}

void register_client(rpc::client *client, int id, string ip, unsigned char* public_key)
{
	client->call("set_client_key", id, ip, get_hex(public_key, crypto_kx_PUBLICKEYBYTES));
}

void send_message()
{
	if(!msgs.empty())
	{
		string message = msgs.front();
		msgs.pop();
		
		int cur_length = message.length();
		cur_length = to_string(cur_length).length()+cur_length+1;
		
		if(cur_length > MESSAGE_LEN)
		{
			cout << "Packed message length greater than MAX_LENGTH\n";
			return;
		}
		
		int remaining = MESSAGE_LEN - cur_length;
		
		message = to_string(cur_length)+"|"+message+string(remaining, ' ');
		
		//cout << "Message is \n" << message << "******\n"; 
	
		unsigned char nonce[crypto_secretbox_NONCEBYTES];
		unsigned char ciphertext[crypto_secretbox_MACBYTES + MESSAGE_LEN];

		randombytes_buf(nonce, sizeof nonce);
		crypto_secretbox_easy(ciphertext, (const unsigned char*)message.c_str(), message.length(), nonce, peer.get_key_e());
		
		string ciphertext_s = get_hex(ciphertext, crypto_secretbox_MACBYTES + MESSAGE_LEN);
	
		//cout << "Ciphertext is " << get_hex(ciphertext, crypto_secretbox_MACBYTES + MESSAGE_LEN) << "\n";
		
		client.client->call("store_message", round.get_label_s(), ciphertext_s);
		
		
		//TODO move lower part when retrieving messages
		
		if (sodium_hex2bin(ciphertext, crypto_secretbox_MACBYTES + MESSAGE_LEN, ciphertext_s.c_str(), ciphertext_s.length(), NULL, NULL, NULL) == -1)
		{
			cout << "Aborting. Peer public key corrupt\n";
		    //return false;
		}

		unsigned char decrypted[MESSAGE_LEN];
		if (crypto_secretbox_open_easy(decrypted, ciphertext, crypto_secretbox_MACBYTES + MESSAGE_LEN, nonce, peer.get_key_e()) != 0) {
			/* message forged! */
		}
		
		cout << "Decrypted is " << string((const char *)decrypted, MESSAGE_LEN) << "\n";		
	}
}

void initialize_new_round(std::string round_id) {

    round.set_round_id(round_id);
    cout << "New round: " << round.get_round_id() << std::endl;
    
    if(peer_joined)
    {
    	create_round_labels();
    	send_message();
    }
}

void create_round_labels()
{
	string s_str = round.get_round_id()+"||"+to_string(peer.get_peer_id());
	string r_str = round.get_round_id()+"||"+to_string(client.get_id());
	
	unsigned char hash[crypto_auth_hmacsha256_BYTES];
	
	crypto_auth_hmacsha256(hash, (const unsigned char *)s_str.c_str(), s_str.length(), peer.get_key_l());
	round.set_label_s(get_hex(hash, sizeof hash));
	//cout << "label_s : " << round.get_label_s() << "\n";
	
	crypto_auth_hmacsha256(hash, (const unsigned char *)r_str.c_str(), r_str.length(), peer.get_key_l());
	round.set_label_r(get_hex(hash, sizeof hash));
	//cout << "label_r : " << round.get_label_r() << "\n";;
	
}

void initialize_client(int id, string master_ip, int master_port)
{
	cout << "\n--------------------\nInitializing Client\n--------------------\n";
	
	unsigned char public_key[crypto_kx_PUBLICKEYBYTES];
    unsigned char private_key[crypto_kx_SECRETKEYBYTES];
    
    cout << "Generating public private keypairs for client...\n";
    crypto_box_keypair(public_key, private_key);
    
    cout << "For client : " << id << "\n";
    cout << "Public key: ";
    cout << get_hex(public_key, sizeof public_key) << "\n";;
    cout << "Private key: ";
    cout << get_hex(private_key, sizeof private_key) << "\n";
    
    rpc::client *rpc_client = new rpc::client(master_ip, master_port);
    string ip = getIPAddress() + ":" + std::to_string(RECEIVE_PORT+id);
    std::cout << ip << std::endl;
    
    register_client(rpc_client, id, ip, public_key); 
    client.init_msg_client(id, ip, rpc_client, master_ip, master_port, public_key, private_key);

    // Setting up rpc server with async callbacks to process rounds information from server
    rpc::server *srv = new rpc::server(RECEIVE_PORT+id);
    srv->bind("rounds_notice", &initialize_new_round);
    srv->async_run(2);
    
    cout << "____________________________________________\n";
}

bool create_comm_keys(int peer_id)
{
	auto key = client.client->call("get_client_key", peer_id).as<std::string>();
	
	if(key.empty())
	{
		cout << "Peer not joined yet. Ask them to join the server or try again later\n";
		return false;
	}
	
	cout << "Peer public key is " << key << "\n";
	
	unsigned char peer_public[crypto_kx_PUBLICKEYBYTES];
	
	if (sodium_hex2bin(peer_public, crypto_kx_PUBLICKEYBYTES, key.c_str(), key.length(), NULL, NULL, NULL) == -1)
	{
		cout << "Aborting. Peer public key corrupt\n";
        return false;
    }	
    
    unsigned char rx[crypto_kx_SESSIONKEYBYTES], tx[crypto_kx_SESSIONKEYBYTES];
    
    int key_res;
    if(client.get_id() < peer_id)
    	key_res = crypto_kx_server_session_keys(rx, tx, 
    											client.get_public_key(), client.get_private_key(),
    											peer_public);
    else if(client.get_id() > peer_id)
    	key_res = crypto_kx_client_session_keys(tx, rx, 
    											client.get_public_key(), client.get_private_key(),
    											peer_public);
    else
    {
    	cout << "Both client ids cannot be same. Exiting try again\n";
    	return false;
    }
    
    if(key_res != 0)
    {
    	cout << "Peer's public key suspicious. Exiting\n";
    	return false;
    }
	
	peer.set_peer_info(client_info(peer_id, "", key));
	peer.set_comm_keys(rx, tx);
	
	cout << "Key_e : " << get_hex(peer.get_key_l(), crypto_kx_SESSIONKEYBYTES) << "\n";
    cout << "Key_l key: " << get_hex(peer.get_key_e(), crypto_kx_SESSIONKEYBYTES) << "\n";
    
    peer_joined = true;
    
    return true;
	
}

void display_help()
{
	cout << "--------------------\nPossible list of commands\n--------------------\n";
	cout << "/join : to enter conversation with a particular client\n";
	cout << "/end  : to end chat with current peer\n";
	cout << "/quit : to quit the client\n";
	cout << "/help : to display this help section again\n";
	cout << "____________________________________________\n";
}

command get_command()
{
	string input;
	getline(cin, input);
	
	if(!input.compare("/join"))
		return JOIN_PEER;
	else if(!input.compare("/end"))
		return QUIT_CHAT;
	else if(!input.compare("/quit"))
		return QUIT_CLIENT;
	else if(!input.compare("/help"))
		return HELP;
	else
	{
		message = input;
		return MSG;
	}
}

void add_to_msgqueue()
{
	msgs.push(message);
	message = "";
}

void destroy_keys_and_data()
{
}

int main(int argc, char **argv) {
	if(argc < 4)
	{
		cout << "Usage: ./client [client_id] [master_ip] [master_port]\n";
		return 0;
	}
	
	init();
			
	int peer_id;
	
	bool run = true;
	command command_id;
	
	initialize_client(atoi(argv[1]), string(argv[2]), atoi(argv[3]));
	
	display_help();
	
	while(run)
	{
		command_id = get_command();
		
		switch(command_id)
		{
			case JOIN_PEER: 	cout << "Enter peer client id\n";
								cin >> peer_id;
								cin.get();
								if(create_comm_keys(peer_id))
								{
									cout << "Communication started with peer " << peer_id << "\n";
									cout << "Type /help anytime to check other command options\n";
								}
								break;
								
			case MSG:			cout << "Adding to msg queue\n";
								add_to_msgqueue();
								break;
								
			case QUIT_CHAT: 	break;
			
			case QUIT_CLIENT: 	cout << "Destroying all private information\n";
								destroy_keys_and_data();
							  	run = false;
							  	break;
							  	
			case HELP:			display_help();
								break;
		}
		
	}

	//TODO - client communication with rounds 
	
	return 0;
}



