//
// Created by Nishanth Prabhu on 06/04/19.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "rpc/client.h"
#include <boost/algorithm/string.hpp>

int main() {
	int master_port = 30000;
	rpc::client client("127.0.0.1", master_port);
	auto result = client.call("set_client_key", 1, "127.0.0.1:8080", "public_key1");
    auto key = client.call("get_client_key", 1).as<std::string>();
    std::cout << "public key: " << key << std::endl;

    unsigned char key2[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    std::string key_string(reinterpret_cast<char*>(key2));
	auto result2 = client.call("set_client_key", 2, "127.0.0.1:8081", key_string);
    auto rcvkeystring = client.call("get_client_key", 2).as<std::string>();
    unsigned char received_key[32];
    //strcpy(static_cast <char*>(received_key), rcvkeystring);
    strcpy((char *)received_key, rcvkeystring.c_str());
    std::cout << "public key received: " << received_key << std::endl;
}



