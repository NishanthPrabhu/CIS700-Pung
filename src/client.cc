//
// Created by Nishanth Prabhu on 06/04/19.
//
	
#include "client.h"
#include "rpc/server.h"

#include "master_callbacks.h"

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

void sigHandler(int sigNum)
{
	if(sigNum == SIGINT ||
	   sigNum == SIGTERM ||
	   sigNum == SIGQUIT ||
	   sigNum == SIGTSTP)
	destroy_keys_and_data();
}

/*!
 * set different signal handlers
 */
void setUpSignals()
{   
     
    //changing the handler for SIGINT
    if(signal(SIGINT, sigHandler) == SIG_ERR)
        perror("signal()");

    //changing the handler for SIGTERM
    if(signal(SIGTERM, sigHandler) == SIG_ERR)
        perror("signal()");

    //changing the handler for SIGTSTP - pause
    if(signal(SIGTSTP, sigHandler) == SIG_ERR)
        perror("signal()");
    
    if(signal(SIGQUIT, sigHandler) == SIG_ERR)
        perror("signal()");
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

void register_client(rpc::client *rpcclient, int id, string ip, string galois_keys)
{
	rpcclient->call("set_client_key", id, ip, client.get_public_key(), galois_keys);
}

void init_new_round(std::string round_id, vector<unsigned char> nonce) {

	//cout << "Round notice\n";
    cur_round.set_round_id(round_id);
    cur_round.set_nonce(nonce);
    
    //cout << "New round: " << cur_round.get_round_id() << "\nNonce : " << get_hex(nonce.data(), nonce.size()) << std::endl;
    
    std::thread thread(&start_send_msg);
    thread.detach();
    
    return;
}

void init_msg_retrieval()
{
	//cout << "Retrieve notice\n";
	std::thread thread(&retrieve_msg);
	thread.detach();
	return;
}

void start_send_msg()
{
	//cout << "Send msg started\n";
	if(peer.join_status())
    {
    	create_round_labels();
   		send_msg();
   	}
    else
    	send_dummy_msg();
    
    //cout << "Send done\n";
}

void create_round_labels()
{
	string s_str = cur_round.get_round_id()+"||"+to_string(peer.get_peer_id());
	string r_str = cur_round.get_round_id()+"||"+to_string(client.get_id());
	
	unsigned char hash[crypto_auth_hmacsha256_BYTES];
	
	crypto_auth_hmacsha256(hash, (const unsigned char *)s_str.c_str(), s_str.length(), peer.get_key_l().data());
	cur_round.set_label_s(get_hex(hash, sizeof hash));
	//cout << "label_s : " << cur_round.get_label_s() << "\n";
	
	crypto_auth_hmacsha256(hash, (const unsigned char *)r_str.c_str(), r_str.length(), peer.get_key_l().data());
	cur_round.set_label_r(get_hex(hash, sizeof hash));
	//cout << "label_r : " << cur_round.get_label_r() << "\n";
	
}

void send_dummy_msg()
{
	unsigned char tmp_s_label[crypto_auth_hmacsha256_BYTES];
    randombytes_buf(tmp_s_label, sizeof tmp_s_label);
    
    string label = get_hex(tmp_s_label, sizeof tmp_s_label);
    
    vector<unsigned char> ciphertext;
	ciphertext.resize(CIPHERTEXT_LEN);
	
	randombytes_buf(ciphertext.data(), ciphertext.size());
    
    client.rpc_client->call("store_message", label, ciphertext);
}

void send_msg()
{
	if(!msgs.empty() && peer.join_status())
	{
		string message;
		
		while(!msgs.empty())
		{
			message.append(msgs.front());
			message.append("\n");
			msgs.pop();
		}
		
		
		
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
		
		vector<unsigned char> ciphertext;
		
		ciphertext.resize(CIPHERTEXT_LEN);
	
		crypto_secretbox_easy(ciphertext.data(),
							 (const unsigned char*)message.c_str(),
							 message.length(),
							 cur_round.get_nonce().data(),
							 peer.get_key_e().data());
		
		client.rpc_client->call("store_message", cur_round.get_label_s(), ciphertext);
	}
	else
		send_dummy_msg();
}

void retrieve_msg()
{
	//cout << "Retrieve started\n";
		
	bool label_found = true;
	//TODO get label mapping
	auto label_map = client.rpc_client->call("get_label_mapping").as<std::map<std::string, int>>();
    int ele_index = -1;

	
	//TODO
	//	- get index from label mapping
	//	- for self testing...change to label_r once pir setup successfully
//	cout << "Looking for label: " << cur_round.get_label_r() << "\n";
    if (label_map.find(cur_round.get_label_r()) != label_map.end())
        ele_index = label_map[cur_round.get_label_r()];
	
//	cout << "Index is " << ele_index << " and label map size is " << label_map.size() << "\n"; 
	
	
	if(ele_index == -1)
	{
		if(label_map.size() != 0)
		{
			ele_index = rd() % label_map.size();
			label_found = false;
		}
		else
			return;
	}
	
	//TODO generate PIR query
	
	uint64_t index = client.pir_client->get_fv_index(ele_index, size_per_item);   // index of FV plaintext
	uint64_t offset = client.pir_client->get_fv_offset(ele_index, size_per_item); // offset in FV plaintext
	
	PirQuery query = client.pir_client->generate_query(index);
	
	//TODO serialize PIR query and send message
	
	auto s_query = serialize_pir_query(query);
	
	string response = client.rpc_client->call("retrieve_message", client.get_id(), s_query).as<string>();
	
	if(label_found)
	{	
		//TODO
		//	- recieve PIR response and decrypt message
		//	- check these values while deserializing
		PirReply reply = deserialize_ciphertexts(NUM_PIR_REPLY_CIPHER, response, CIPHER_SIZE);
		
		Plaintext result = client.pir_client->decode_reply(reply);
		
//		cout << result.to_string();
		
		vector<unsigned char> ciphertext(N * logt / 8);
	    coeffs_to_bytes(logt, result, ciphertext.data(), (N * logt) / 8);

		unsigned char decrypted[MESSAGE_LEN];
		
		if (crypto_secretbox_open_easy(decrypted,
									   ciphertext.data()+(offset*size_per_item),
									   CIPHERTEXT_LEN,
									   cur_round.get_nonce().data(),
									   peer.get_key_e().data()) != 0) {
			cout << "WARNING forged message recieved...\n";
			return;
		}
		
		string decrypt_msg = string((const char *)decrypted);
		int sep = decrypt_msg.find('|');
		int msg_length = atoi(decrypt_msg.substr(0, 2).c_str());
		decrypt_msg = decrypt_msg.substr(sep+1, msg_length - (sep+1));
		
		stringstream stream(decrypt_msg);
		cout << "\r";
		while(getline(stream, response))
			cout << "peer : " << response << "\n";
			
		cout << "me : " << std::flush;
		
	}
	
	//cout << "Done retrieving\n";
}

void initialize_client(int id, string master_ip, int master_port)
{
	cout << "\n--------------------\nInitializing Client\n--------------------\n";
    
    cout << "Generating public private keypairs for client...\n";
    crypto_box_keypair(client.get_public_key().data(), client.get_private_key().data());
    
    cout << "For client : " << id << "\n";
    cout << "Public key: ";
    cout << get_hex(client.get_public_key().data(), client.get_public_key().size()) << "\n";;
    cout << "Private key: ";
    cout << get_hex(client.get_private_key().data(), client.get_private_key().size()) << "\n";
    
    
    //Set up rpc_client
    rpc::client *rpc_client = new rpc::client(master_ip, master_port);
    string ip = getIPAddress() + ":" + std::to_string(RECEIVE_PORT+id);
    std::cout << ip << std::endl;

    // Set up for PIR
    
    EncryptionParameters params(scheme_type::BFV);
    PirParams pir_params;
    gen_params(number_of_items, size_per_item, N, logt, d, params, pir_params);

    PIRClient *pir_client = new PIRClient(params, pir_params);
    
    GaloisKeys galois_keys = pir_client->generate_galois_keys();
    
    register_client(rpc_client, id, ip, serialize_galoiskeys(galois_keys)); 
    
    client.init_msg_client(id, ip, rpc_client, pir_client, master_ip, master_port);
    
//    cout << "___________________________________________\n";
//    cout << "Trying PIR Query counts\n";
//    
//    int ele_index = 3;
//    
//    uint64_t index = pir_client->get_fv_index(ele_index, size_per_item);   // index of FV plaintext
//    uint64_t offset = pir_client->get_fv_offset(ele_index, size_per_item); // offset in FV plaintext
//    	
//    PirQuery query = pir_client->generate_query(index);
//    
//    for(auto v : query)
//    	cout << v.size() << "\n";
//    
//    auto vec = serialize_pir_query(query);
//    
//    for(auto str : vec)
//    	cout << str.length() << "\n";
//    	
//    query = deseralize_pir_query(vec);
//    
//    for(auto v : query)
//    	cout << v.size() << "\n";
//    	
//    cout << "___________________________________________\n";

    // Setting up rpc server with async callbacks to process rounds information from server
    rpc::server *srv = new rpc::server(RECEIVE_PORT+id);
    srv->bind("rounds_notice", &init_new_round);
    srv->bind("retrieve_notice", &init_msg_retrieval);
    srv->async_run(2);
    
    cout << "____________________________________________\n";
}

bool create_comm_keys(int peer_id)
{
	auto key = client.rpc_client->call("get_client_key", peer_id).as<vector<unsigned char>>();
	
	if(key.size() == 0)
	{
		cout << "Peer not joined yet. Ask them to join the server or try again later\n";
		return false;
	}
	
	cout << "Peer public key is " << get_hex(key.data(), crypto_kx_PUBLICKEYBYTES) << "\n";
	    
    int key_res;
    if(client.get_id() < peer_id)
    	key_res = crypto_kx_server_session_keys(peer.get_key_l().data(), peer.get_key_e().data(), 
    											client.get_public_key().data(), client.get_private_key().data(),
    											key.data());
    else if(client.get_id() > peer_id)
    	key_res = crypto_kx_client_session_keys(peer.get_key_e().data(), peer.get_key_l().data(), 
    											client.get_public_key().data(), client.get_private_key().data(),
    											key.data());
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
	
	cout << "Key_e : " << get_hex(peer.get_key_l().data(), crypto_kx_SESSIONKEYBYTES) << "\n";
    cout << "Key_l : " << get_hex(peer.get_key_e().data(), crypto_kx_SESSIONKEYBYTES) << "\n";
    
	peer.set_peer_info(client_info(peer_id, "", key, NULL));
    
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
	
	cout << "me : ";
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
		return MSG;
}

void add_to_msgqueue()
{
	msgs.push(input);
	input = "";
}

void remove_peer()
{
	peer.clear_peer_info();
}

void destroy_keys_and_data()
{
	client.rpc_client->call("shutdown_client", client.get_id());
	peer.clear_peer_info();
	client.clear_client();
	
	run = false;
}

int main(int argc, char **argv) {

	if(argc < 4)
	{
		cout << "Usage: ./client [client_id] [master_ip] [master_port]\n";
		return 0;
	}
	
	init();
	
	setUpSignals();
			
	int peer_id;
	
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
								
			case MSG:			if(!peer.join_status())
									cout << "Join a peer first to start communication\n";
								else
									add_to_msgqueue();
								break;
								
			case QUIT_CHAT: 	//TODO
								//	- handling when client quit between conversation
								//  - send quit message to oth client
								remove_peer();
								break;
			
			case QUIT_CLIENT: 	cout << "Destroying all private information\n";
								destroy_keys_and_data();
							  	break;
							  	
			case HELP:			display_help();
								break;
		}
		
	}
	
	return 0;
}

