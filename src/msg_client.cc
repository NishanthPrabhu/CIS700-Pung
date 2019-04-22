//
// Created by Nishanth Prabhu on 27/03/19.
//

#include "msg_client.h"

msg_client::msg_client()
{
	rpc_client = NULL;
	pir_client = NULL;
	public_key.resize(crypto_kx_PUBLICKEYBYTES);
	private_key.resize(crypto_kx_SECRETKEYBYTES);
}

void msg_client::init_msg_client(int id,
						   string ip,
						   rpc::client *r_client,
						   PIRClient *p_client,
						   string master_ip,
						   int master_port) {

	this->id = id;   
	this->ip = ip;
	this->rpc_client = r_client;
	this->pir_client = p_client;
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

void msg_client::clear_client()
{
	sodium_memzero(public_key.data(), public_key.size());
	sodium_memzero(private_key.data(), private_key.size());
	
	if(rpc_client != NULL)
		delete rpc_client;
	
	if(pir_client != NULL)
		delete pir_client;
}

msg_peer::msg_peer()
{
	peer_joined = false;
	key_l.resize(crypto_kx_SESSIONKEYBYTES);
	key_e.resize(crypto_kx_SESSIONKEYBYTES);
}
void msg_peer::set_peer_info(client_info info) {
    this->peer = info;
    peer_joined = true;
}

void msg_peer::clear_peer_info()
{
	this->peer.clear_client_info();
	peer_joined = false;
	sodium_memzero(key_l.data(), key_l.size());
	sodium_memzero(key_e.data(), key_e.size());
}

bool msg_peer::join_status()
{
	return peer_joined;
}

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
	label_s.clear();
	label_r.clear();
}

string round_info::get_round_id()
{
	return round_id;
}

void round_info::set_nonce(vector<unsigned char> nonce)
{
	this->nonce = nonce;
}

vector<unsigned char>& round_info::get_nonce()
{
	return nonce;
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
