//
// Created by Nishanth Prabhu on 28/03/19.
//

#include <iostream>
#include "slave_callbacks.h"
#include "slave_server.h"
#include "server_info.h"
#include "rpc/client.h"

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
void set_and_propagate_client_public_key(int client_id, std::string publickey) {
	keys_map[client_id] = publickey;

	for (server_info info : slaves) {
		if (!info.get_server_name().compare(slave_name)) {
			continue;
		}

		std::string slave_ip = info.get_server_ip();
		int port = info.get_port();
		rpc::client client(slave_ip, port);
		auto result = client.call("set_client_key", client_id, publickey).as<bool>();
	}
}

std::string get_public_key(std::string client_id) {
	// TODO get from local map and return
	std::string result;
	return result;
}

bool store_message(std::string label, std::string message) {
	// TODO Store message in local message map
	bool result;
	return result;
}

// This needs PIR, how to integrate?
std::string retrieve_message() {
	return "";
}