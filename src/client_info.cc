//
// Created by Nishanth Prabhu on 27/03/19.
//

#include "client_info.h"
#include <vector>

client_info::client_info(){};

client_info::client_info(int client_id, std::string const& client_ip,
                         std::vector<unsigned char> const& publickey, std::string const& galoiskey) {
	this->client_id = client_id;
	this->client_ip = client_ip;
	this->publickey = publickey;
    this->galoiskey = galoiskey;
}

int client_info::get_client_id() {
	return client_id;
}

string client_info::get_client_ip() {
	return client_ip;
}

std::vector<unsigned char> client_info::get_public_key() {
	return publickey;
}

std::string client_info::get_galois_key() {
    return galoiskey;
}
