//
// Created by Nishanth Prabhu on 27/03/19.
//

#include "msg_client.h"

void msg_client::init_msg_client(int id,
						   string ip,
						   rpc::client *client,
						   string master_ip,
						   int master_port,
						   const unsigned char* public_key,
                           const unsigned char* private_key) {

	this->id = id;   
	this->ip = ip;
	this->client = client;
	this->master_ip = master_ip;
	this->master_port = master_port;                         
	memcpy(this->public_key, public_key, crypto_kx_PUBLICKEYBYTES);
	memcpy(this->private_key, private_key, crypto_kx_PUBLICKEYBYTES);
}

int msg_client::get_id() {
    return id;
}

string msg_client::get_ip() {
    return ip;
}

string msg_client::get_master_ip() {
    return master_ip;
}

int msg_client::get_master_port() {
    return master_port;
}

unsigned char* msg_client::get_public_key() {
    return public_key;
}

unsigned char* msg_client::get_private_key() {
    return private_key;
}

void msg_peer::set_peer_info(client_info info) {
    this->peer = info;
}

void msg_peer::set_comm_keys(const unsigned char* recieve_key,
                              const unsigned char* transmit_key) {

    memcpy(this->recieve_key, recieve_key, crypto_kx_SESSIONKEYBYTES);
	memcpy(this->transmit_key, transmit_key, crypto_kx_SESSIONKEYBYTES);
}

unsigned char* msg_peer::get_recieving_key() {
    return recieve_key;
}
unsigned char* msg_peer::get_transmission_key() {
    return transmit_key;
}

