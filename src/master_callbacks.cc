//
// Created by Nishanth Prabhu on 28/03/19.
//

#include <iostream>
#include "master_callbacks.h"
#include "master_server.h"
#include "server_info.h"

/**
 *
 * @param client_id
 * @param publickey
 * @return
 *
 * IMPORTANT: client has to convert libsodiums unsigned char[] public key
 * to a string before sending. easier than dealing with msgpack directly.
 * Size is 32 bytes, so unpacking into a unsigned char array ourselves is easy.
 */
bool set_client_public_key(int client_id, std::string publickey) {
	int slave_index = get_slave_index();
	return false;
}

std::string get_client_public_key(std::string client_id) {
	return NULL;
}

bool register_client(int client_id) {
	return true;
}

bool store_client_message(std::string message) {
	int slave_index = get_slave_index();
	return false;
}

std::string retrieve_client_message() {
	return "";
}

