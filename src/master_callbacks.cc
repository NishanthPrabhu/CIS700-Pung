//
// Created by Nishanth Prabhu on 28/03/19.
//

#include <iostream>
#include "master_callbacks.h"
#include "master_server.h"
#include "server_info.h"
#include "rpc/msgpack.hpp"
#include "rpc/client.h"
#include "rpc/rpc_error.h"
#include <boost/algorithm/string.hpp>
#include <vector>

std::map<int, std::string> client_address_map;
//std::vector<std::tuple<std::string, int>> label_map;
std::map<std::string, int> label_map;

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
            const uint64_t short_timeout = 7000;
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

std::vector<unsigned char> get_client_public_key(int client_id) {

    std::vector<unsigned char> result;
    while (true) {
        int slave_index = get_slave_index();
	    std::string slave_ip = slaves[slave_index].get_server_ip();
	    int port = slaves[slave_index].get_port();
        boost::trim(slave_ip);
	    rpc::client client(slave_ip, port);
	
        try {
            const uint64_t short_timeout = 2000;
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

std::map<std::string, int> get_label_mapping() {
    return label_map;
}    

void test_unsigned_char(std::vector<unsigned char> buff) {
    RPCLIB_MSGPACK::sbuffer sbuf;
    RPCLIB_MSGPACK::packer<RPCLIB_MSGPACK::sbuffer> packer(sbuf);
}

void store_client_message(std::string label, std::vector<unsigned char>const& message) {
  	int slave_index = get_slave_index();
	std::string slave_ip = slaves[slave_index].get_server_ip();
    boost::trim(slave_ip);
    int port = slaves[slave_index].get_port();

    int index = get_label_index();
    label_map[label] = index;
    std::cout << "Storing label: " << label << " at index: " << index << std::endl;
    rpc::client client(slave_ip, port);
	client.call("store_and_propagate_message", index, label, message);
}

// This needs PIR, how to integrate?
std::string retrieve_message(int client_id, std::vector<std::string> serializedQuery)
{
  	return "";
}
