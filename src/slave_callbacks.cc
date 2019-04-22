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
#include "SealPIR/pir.hpp"
#include <boost/algorithm/string.hpp>

std::map<int, client_info> keys_map;
std::string current_round;
auto db(make_unique<uint8_t[]>(number_of_items * size_per_item));

using namespace seal;

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
	client_info info(client_id, client_ip, publickey, deserialize_galoiskeys(galoiskey));
    keys_map[client_id] = info;
}

void set_and_propagate_client_public_key(int client_id, std::string const& client_ip,
                                    std::vector<unsigned char> const& publickey, std::string const& galoiskey) {
	client_info info(client_id, client_ip, publickey, deserialize_galoiskeys(galoiskey));
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
            client.call("set_client_key", client_id, client_ip, publickey, galoiskey);
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
    return -1;
}    

void initialize_new_round(std::string round_id) {
    std::cout << "New round alert" << std::endl;
    current_round = round_id;
    db.reset();
    // TODO check if this is the right thing to do
    db = make_unique<uint8_t[]>(number_of_items * size_per_item);
}

void store_message(int index, std::vector<unsigned char> const& label, std::vector<unsigned char> const& message) {
    // TODO store message at index
}

void store_and_propagate_message(int index, std::vector<unsigned char> const& label, std::vector<unsigned char> const& message) {
	bool result;
    // TODO store message at index locally

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
std::string retrieve_message(int client_id, std::vector<std::string> serializedQuery) {
    // Get galois keys of client from map
    GaloisKeys* galois_keys = keys_map[client_id].get_galois_keys(); 
    EncryptionParameters params(scheme_type::BFV);
    PirParams pir_params;
    gen_params(number_of_items, size_per_item, N, logt, d, params, pir_params);
    PIRServer server(params, pir_params);
    server.set_galois_key(0, *galois_keys);

    // Deserialize the query
    ssize_t encrypted_count = 2;

    std::vector< std::vector<seal::Ciphertext> > pir_query;
    for (int i = 0; i < serializedQuery.size(); i++) {
        std::string cipher = serializedQuery[i];
        pir_query.push_back(deserialize_ciphertexts(encrypted_count, cipher, CIPHERTEXT_LEN)); 
    }

    // TODO process query and serialized pirreply to string
}
