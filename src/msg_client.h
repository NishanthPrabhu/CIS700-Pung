//
// Created by Nishanth Prabhu on 27/03/19.
//

#include <sodium.h>
#include <string>

#include "client_info.h"
#include <sodium.h>
#include "rpc/client.h"

using namespace std;

#ifndef CIS700_PUNG_MSG_CLIENT_H
#define CIS700_PUNG_MSG_CLIENT_H


class msg_client {

	int id;
	string ip;
	string master_ip;
	int master_port;
	unsigned char public_key[crypto_kx_PUBLICKEYBYTES];
    unsigned char private_key[crypto_kx_SECRETKEYBYTES];
    

public:
	rpc::client *client;
	void init_msg_client(int,string, rpc::client*, string, int,
					const unsigned char*, const unsigned char*);
	int get_id();
	string get_ip();
	string get_master_ip();
	int get_master_port();
	unsigned char* get_public_key();
	unsigned char* get_private_key();
    
};

class msg_peer {

    client_info peer;
    unsigned char recieve_key[crypto_kx_SESSIONKEYBYTES];
    unsigned char transmit_key[crypto_kx_SESSIONKEYBYTES];
    
public:
    void set_peer_info(client_info);
    void set_comm_keys(const unsigned char*, const unsigned char*);
    unsigned char* get_recieving_key();
    unsigned char* get_transmission_key();
};


#endif //CIS700_PUNG_CLIENT_INFO_H
