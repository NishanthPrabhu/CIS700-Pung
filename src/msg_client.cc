//
// Created by Nishanth Prabhu on 27/03/19.
//

#include "msg_client.h"

msg_client::msg_client()
{
	public_key.resize(crypto_kx_PUBLICKEYBYTES);
	private_key.resize(crypto_kx_SECRETKEYBYTES);
}

void msg_client::init_msg_client(int id,
						   string ip,
						   rpc::client *client,
						   string master_ip,
						   int master_port) {

	this->id = id;   
	this->ip = ip;
	this->client = client;
	this->master_ip = master_ip;
	this->master_port = master_port;
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

vector<unsigned char>& msg_client::get_public_key() {
    return public_key;
}

vector<unsigned char>& msg_client::get_private_key() {
    return private_key;
}

msg_peer::msg_peer()
{
	key_l.resize(crypto_kx_SESSIONKEYBYTES);
	key_e.resize(crypto_kx_SESSIONKEYBYTES);
}
void msg_peer::set_peer_info(client_info info) {
    this->peer = info;
}

//void msg_peer::set_comm_keys(const unsigned char* recieve_key,
//                              const unsigned char* transmit_key) {

//    memcpy(this->key_l, recieve_key, crypto_kx_SESSIONKEYBYTES);
//	memcpy(this->key_e, transmit_key, crypto_kx_SESSIONKEYBYTES);
//}

vector<unsigned char>& msg_peer::get_key_l() {
    return key_l;
}

vector<unsigned char>& msg_peer::get_key_e() {
    return key_e;
}

int msg_peer::get_peer_id()
{
	return peer.get_client_id();
}


void round_info::set_round_id(string id)
{
	round_id = id;
}

string round_info::get_round_id()
{
	return round_id;
}

void round_info::set_label_s(string label)
{
	label_s = label;
}

string round_info::get_label_s()
{
	return label_s;
}

void round_info::set_label_r(string label)
{
	label_r = label;
}

string round_info::get_label_r()
{
	return label_r;
}
