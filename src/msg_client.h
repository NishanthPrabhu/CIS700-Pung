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
	vector<unsigned char> public_key;
    vector<unsigned char> private_key;
    

public:
	rpc::client *client;
	msg_client();
	void init_msg_client(int,string, rpc::client*, string, int);
	int get_id();
	string get_ip();
	string get_master_ip();
	int get_master_port();
	vector<unsigned char>& get_public_key();
	vector<unsigned char>& get_private_key();
    
};

class msg_peer {

    client_info peer;
    vector<unsigned char> key_l;
    vector<unsigned char> key_e;
    
public:
	msg_peer();
    void set_peer_info(client_info);
    //void set_comm_keys(const unsigned char*, const unsigned char*);
    vector<unsigned char>& get_key_l();
    vector<unsigned char>& get_key_e();
    int get_peer_id();
};

class round_info {
	
	string round_id;
	string label_s;
	string label_r;
	
public:
	void set_round_id(string id);
	string get_round_id();
	
	void set_label_s(string label);
	string get_label_s();
	
	void set_label_r(string label);
	string get_label_r();
};


#endif //CIS700_PUNG_CLIENT_INFO_H
