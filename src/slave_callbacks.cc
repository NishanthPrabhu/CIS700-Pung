//
// Created by Nishanth Prabhu on 28/03/19.
//

#include <iostream>
#include <tuple>
#include "slave_callbacks.h"
#include "slave_server.h"
#include "server_info.h"
#include "client_info.h"
#include "rpc/client.h"
#include "rpc/rpc_error.h"
#include <boost/algorithm/string.hpp>

std::map<int, client_info> keys_map;
std::string current_round;
std::vector<std::tuple<std::string, std::string>> message_store; 

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
void set_client_public_key(int client_id, std::string const& client_ip,
                           std::vector<unsigned char> const& publickey, std::string const& galoiskey) {
	client_info info(client_id, client_ip, publickey, galoiskey);
    keys_map[client_id] = info;
}

void set_and_propagate_client_public_key(int client_id, std::string const& client_ip,
                                    std::vector<unsigned char> const& publickey, std::string const& galoiskey) {
	client_info info(client_id, client_ip, publickey, galoiskey);
    keys_map.emplace(client_id, info);

	for (server_info info : slaves) {
		if (!info.get_server_name().compare(slave_name)) {
			continue;
		}

		std::string slave_ip = info.get_server_ip();
        boost::trim(slave_ip);
        int port = info.get_port();
		rpc::client client(slave_ip, port);
	    
        try {
            const uint64_t short_timeout = 1000;
            client.set_timeout(short_timeout);
            client.call("set_client_key", client_id, client_ip, publickey);
        } catch (rpc::timeout &t) {
            std::cout << "Slave not responding..skip" << std::endl;    
        }    
    }
}

std::vector<unsigned char> get_public_key(int client_id) {
	std::string result;

	std::cout << "Returning result\n";
	if (keys_map.count(client_id) > 0) {
	    client_info info = keys_map.find(client_id)->second;
        return info.get_public_key();
    }

	return std::vector<unsigned char>();
}

int send_index_vote() {
    int current_index = message_store.size() - 1;
    return current_index + 1;
}    

void initialize_new_round(std::string round_id) {
    std::cout << "New round alert" << std::endl;
    current_round = round_id;
    message_store.clear();
}

void store_message(int index, std::string label, std::string message) {
    if (message_store.size() <= index) {
        message_store.resize(index+1);
    }
    message_store.at(index) = std::make_tuple(label, message);
}

void store_and_propagate_message(int index, std::string label, std::string message) {
	bool result;
    if (message_store.size() <= index) {
        message_store.resize(index+1);
    }
    message_store.at(index) = std::make_tuple(label, message);

	for (server_info info : slaves) {
		if (!info.get_server_name().compare(slave_name)) {
			continue;
		}

		std::string slave_ip = info.get_server_ip();
        boost::trim(slave_ip);
        int port = info.get_port();
		rpc::client client(slave_ip, port);
	    
        try {
            const uint64_t short_timeout = 1000;
            client.set_timeout(short_timeout);
            client.call("store_message", index, label, message);
        } catch (rpc::timeout &t) {
            std::cout << "Slave not responding..skip" << std::endl;    
        }    
    }
}

// This needs PIR, how to integrate?
std::string retrieve_message() {
	return "";
}
