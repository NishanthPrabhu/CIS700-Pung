//
// Created by Nishanth Prabhu on 28/03/19.
//

#include <iostream>
#include "master_callbacks.h"
#include "master_server.h"
#include "utils/server_info.h"
#include "rpc/msgpack.hpp"
#include "rpc/client.h"
#include "rpc/rpc_error.h"
#include <boost/algorithm/string.hpp>
#include <vector>

std::map<int, std::string> client_address_map;
std::map<std::string, int> label_map;

/**
 *
 * @param client_id
 * @param publickey
 * @return
 *
 */
 
// master chooses a slave and passes the params to it to save on all slaves
bool set_client_public_key(int client_id, std::string const& client_ip,
                           std::vector<unsigned char> const& publickey, std::string const& galoiskey) {
	std::cout<<"Setting public key"<<std::endl;
    client_address_map[client_id] = client_ip;

    while (true) {
        int slave_index = get_slave_index();
        std::string slave_ip = slaves[slave_index].get_server_ip();
  	    int port = slaves[slave_index].get_port();
        boost::trim(slave_ip);
  	    rpc::client client(slave_ip, port);
  	  
        try {
            const uint64_t short_timeout = 500;
            client.set_timeout(short_timeout);
            client.call("set_and_propagate_client_key", client_id, client_ip, publickey, galoiskey);
        } catch (rpc::timeout &t) {
            std::cout << "Slave not responding..skip" << std::endl;
            continue;
        }

        break;
    }
        
    return true;
}

// returns the client_public_key given its id
std::vector<unsigned char> get_client_public_key(int client_id) {

    std::vector<unsigned char> result;
    while (true) {
        int slave_index = get_slave_index();
	    std::string slave_ip = slaves[slave_index].get_server_ip();
	    int port = slaves[slave_index].get_port();
        boost::trim(slave_ip);
	    rpc::client client(slave_ip, port);
	
        try {
            const uint64_t short_timeout = 500;
            client.set_timeout(short_timeout);
            std::cout << "Calling client\n";
            result = client.call("get_client_key", client_id).as<std::vector<unsigned char>>();
        } catch (rpc::timeout &t) {
            std::cout << "Slave not responding..skip" << std::endl;
            continue;
        }
        break;
    }
    std::cout << "Result recieved...returning it\n";
    return result;
}

// returns the label mapping during retrieve round when asked by clients
std::map<std::string, int> get_label_mapping() {
    return label_map;
}    

// stores the label and the client encrypted message on the slave server
void store_client_message(std::string label, std::vector<unsigned char>const& message) {
  	
    while (true) {
        int slave_index = get_slave_index();
	    std::string slave_ip = slaves[slave_index].get_server_ip();
        boost::trim(slave_ip);
        int port = slaves[slave_index].get_port();

        int index = get_label_index();
        label_map[label] = index;
        std::cout << "Storing label: " << label << " at index: " << index << std::endl;
        rpc::client client(slave_ip, port);
	    try {
	        const uint64_t short_timeout = 500;
            client.set_timeout(short_timeout);
            client.call("store_and_propagate_message", index, label, message);
        } catch (rpc::timeout &t) {
            continue;
        }
        break;
    }
}

// retrieves the PIRReply for the serialized PIRQuery sent by the client
std::string retrieve_message(int client_id, std::vector<std::string> serializedQuery)
{
	while (true) {
        int slave_index = get_slave_index();
    	std::string slave_ip = slaves[slave_index].get_server_ip();
        boost::trim(slave_ip);
        int port = slaves[slave_index].get_port();
        rpc::client client(slave_ip, port);
        try {
	        const uint64_t short_timeout = 500;
            client.set_timeout(short_timeout);
            return client.call("retrieve_client_message", client_id, serializedQuery).as<std::string>();
        } catch (rpc::timeout &t) {
            continue;
        }
        break;
    }
}

// removes clients public keys when client is shutdown
void shutdown_client(int client_id) {
    for (int i = 0; i < slaves.size(); i++) {
        server_info slave = slaves[i];
        std::string slave_ip = slave.get_server_ip();
        int port = slave.get_port();
        boost::trim(slave_ip);
        rpc::client client(slave_ip, port);

        try {
            const uint64_t short_timeout = 1000;
            client.set_timeout(short_timeout);
            client.call("client_shutdown", client_id);
        } catch (rpc::timeout &t) {
            std::cout << "Slave not responding..skip" << std::endl;
            continue;
        }
    }
    client_address_map.erase(client_id);
}
