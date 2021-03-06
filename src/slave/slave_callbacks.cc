//
// Created by Nishanth Prabhu on 28/03/19.
//

#include <iostream>
#include <tuple>
#include <memory>
#include "slave_callbacks.h"
#include "slave_server.h"
#include "utils/server_info.h"
#include "utils/client_info.h"
#include "rpc/client.h"
#include "rpc/rpc_error.h"
#include "SealPIR/utils/pir.hpp"
#include "SealPIR/server/pir_server.hpp"
#include <boost/algorithm/string.hpp>

std::map<int, client_info> keys_map;
std::string current_round;
int available_index = 0;
std::unique_ptr<unsigned char[]> db;

EncryptionParameters *params;
PirParams pir_params;
PIRServer *server;

using namespace seal;

//removes the clients info when client issues shutdown notice
void shutdown_client(int client_id) {
    if (keys_map.find(client_id) != keys_map.end()) {
        client_info &info = keys_map[client_id];
        info.clear_client_info();
        keys_map.erase(client_id);
    }
}

/**
 *
 * @param client_id
 * @param publickey
 * @return
 *
 */
// sets clients client_id, public key, galois key and ip in its structure
void set_client_public_key(int client_id, std::string const& client_ip,
                           std::vector<unsigned char> const& publickey, std::string const& galoiskey) {
	client_info info(client_id, client_ip, publickey, deserialize_galoiskeys(galoiskey));
    keys_map[client_id] = info;
}

//sets client_info on own machine and propogates it to other clients
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
            const uint64_t short_timeout = 500;
            client.set_timeout(short_timeout);
            client.call("set_client_key", client_id, client_ip, publickey, galoiskey);
        } catch (rpc::timeout &t) {
            std::cout << "Slave not responding..skip" << std::endl;
            continue;
        }    
    }
}

// return public key of a particular client
std::vector<unsigned char> get_public_key(int client_id) {
	std::string result;

	std::cout << "Returning result\n";
	if (keys_map.count(client_id) > 0) {
	    client_info info = keys_map.find(client_id)->second;
        return info.get_public_key();
    }

	return std::vector<unsigned char>();
}

// returns the highest index available for label storage
int send_index_vote() {
    return available_index;
}    

// resets structures when new round initialized
void initialize_new_round(std::string round_id) {
    std::cout << "New round alert" << std::endl;
    current_round = round_id;
    available_index = 0;
    db = std::make_unique<unsigned char[]>(number_of_items * size_per_item);
}

// stores message in local structures
void store_message(int index,std::string const& label, std::vector<unsigned char> const& message) {
    available_index = index + 1;
    std::cout << "Label: " << label << std::endl;
    std::cout << "Message size: " << message.size() << std::endl;
    std::cout << "Expected message size: " << size_per_item << std::endl;
    /*for (uint64_t j = 0; j < size_per_item; j++) {
        db.get()[(index*size_per_item) + j] = message.data()[j];
    }*/   
    
    memcpy(&db.get()[index*size_per_item], message.data(), size_per_item);
    std::cout << "Message stored successfully" << std::endl;
}

// stores the label and message in local structures and propogates them to other slaves
void store_and_propagate_message(int index, std::string const& label, std::vector<unsigned char> const& message) {
	bool result;
    store_message(index, label, message);

	for (server_info info : slaves) {
		if (!info.get_server_name().compare(slave_name)) {
			continue;
		}

		std::string slave_ip = info.get_server_ip();
        boost::trim(slave_ip);
        int port = info.get_port();
		rpc::client client(slave_ip, port);
	    
        try {
            const uint64_t short_timeout = 500;
            client.set_timeout(short_timeout);
            client.call("store_message", index, label, message);
        } catch (rpc::timeout &t) {
            std::cout << "Slave not responding..skip" << std::endl;    
            continue;
        }    
    }
}

// preprocesses database to get ready for retrieval requests
void start_retrieve_stage(std::string round_number) {
    server->set_database(db, number_of_items, size_per_item);
    server->preprocess_database();
}

// initialized PIR database adn server
void initialize_pir() {
    params = new EncryptionParameters(scheme_type::BFV);
    gen_params(number_of_items, size_per_item, N, logt, d, *params, pir_params);
    server = new PIRServer(*params, pir_params); 
    db = std::make_unique<unsigned char[]>(number_of_items * size_per_item);
}    

// process serialized PIRQuery and responds with a serialized PIRReply
std::string retrieve_message(int client_id, std::vector<std::string> serializedQuery) {
    // Get galois keys of client from map
    GaloisKeys* galois_keys = keys_map[client_id].get_galois_keys(); 
    //server->set_database(db, number_of_items, size_per_item);
    //server->preprocess_database();
    server->set_galois_key(client_id, *galois_keys);

    PirQuery query = deseralize_pir_query(serializedQuery);
    PirReply reply = server->generate_reply(query, client_id);
    std::string serialized_reply = serialize_ciphertexts(reply);
    return serialized_reply;
}
