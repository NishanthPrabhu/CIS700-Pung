//
// Created by Nishanth Prabhu on 27/03/19.
//

#include "client_info.h"


client_info::client_info(){};

client_info::client_info(int client_id, std::string const& client_ip, std::string const& publickey) {
	this->client_id = client_id;
	this->client_ip = client_ip;
	this->publickey = publickey;
}

int client_info::get_client_id() {
	return client_id;
}

string client_info::get_client_ip() {
	return client_ip;
}

string client_info::get_public_key() {
	return publickey;
}
