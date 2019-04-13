//
// Created by Nishanth Prabhu on 06/04/19.
//

#include "client.h"
#include "rpc/server.h"

#define RECEIVE_PORT 40000

std::string current_round_id;

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

void initialize_new_round(std::string round_id) {
    cout << "New round: " << round_id << std::endl;
    current_round_id = round_id;
}    

void initialize_client(msg_client &client, int id, string master_ip, int master_port) {
	
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
    string ip = getIPAddress() + ":" + std::to_string(RECEIVE_PORT);
    std::cout << ip << std::endl;
    register_client(rpc_client, id, ip, public_key); 
    client.init_msg_client(id, ip, rpc_client, master_ip, master_port, public_key, private_key);

    // Setting up rpc server with async callbacks to process rounds information from server
    rpc::server *srv = new rpc::server(RECEIVE_PORT);
    srv->bind("rounds_notice", &initialize_new_round);
    srv->async_run(2);
}

void create_comm_keys(msg_client &client, msg_peer &peer, int peer_id)
{
	auto key = client.client->call("get_client_key", peer_id).as<std::string>();
	
	cout << "Peer public key is " << key << "\n";
	
	unsigned char peer_public[crypto_kx_PUBLICKEYBYTES];
	
	if (sodium_hex2bin(peer_public, crypto_kx_PUBLICKEYBYTES, key.c_str(), key.length(), NULL, NULL, NULL) == -1)
	{
		cout << "Aborting. Peer public key corrupt\n";
        abort();
    }	
    
    unsigned char rx[crypto_kx_SESSIONKEYBYTES], tx[crypto_kx_SESSIONKEYBYTES];
    
    int key_res;
    if(client.get_id() < peer_id)
    	key_res = crypto_kx_server_session_keys(rx, tx, 
    											client.get_public_key(), client.get_private_key(),
    											peer_public);
    else if(client.get_id() > peer_id)
    	key_res = crypto_kx_client_session_keys(rx, tx, 
    											client.get_public_key(), client.get_private_key(),
    											peer_public);
    else
    {
    	cout << "Both client ids cannot be same. Exiting try again\n";
    	abort();
    }
    
    if(key_res != 0)
    {
    	cout << "Peer's public key suspicious. Exiting\n";
    	abort();
    }
	
	peer.set_peer_info(client_info(peer_id, "", key));
	peer.set_comm_keys(rx, tx);
	
	cout << "Recieving Key: " << get_hex(peer.get_recieving_key(), crypto_kx_SESSIONKEYBYTES) << "\n";
    cout << "Sending key: " << get_hex(peer.get_transmission_key(), crypto_kx_SESSIONKEYBYTES) << "\n";
	
}

int main(int argc, char **argv) {
	if(argc < 4)
	{
		cout << "Usage: ./client [client_id] [master_ip] [master_port]\n";
		return 0;
	}
	
	init();
			
	msg_client client;
	msg_peer peer;
	int peer_id;
	
	initialize_client(client, atoi(argv[1]), string(argv[2]), atoi(argv[3]));

	cout << "Enter peer client id\n";
	
	cin >> peer_id;
	
	create_comm_keys(client, peer, peer_id);	
	
	//TODO - client communication with rounds 
	
	return 0;
}



