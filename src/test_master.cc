//
// Created by Nishanth Prabhu on 06/04/19.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "rpc/client.h"
#include <boost/algorithm/string.hpp>
using namespace::std;

int main() {
	int master_port = 30000;
	rpc::client client("127.0.0.1", master_port);

    std::chrono::milliseconds timespan(2000);
    
    auto result = client.call("set_client_key", 1, "127.0.0.1:8080", "public_key1");
    //std::this_thread::sleep_for(timespan);
    result = client.call("set_client_key", 1, "127.0.0.1:8080", "public_key2");
    auto key = client.call("get_client_key", 1).as<std::string>();
    std::cout << "public key: " << key << std::endl;

    unsigned char key2[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    std::string key_string(reinterpret_cast<char*>(key2));
	auto result2 = client.call("set_client_key", 2, "127.0.0.1:8081", key_string);
    //std::this_thread::sleep_for(timespan);
    auto rcvkeystring = client.call("get_client_key", 2).as<std::string>();
    unsigned char received_key[32];
    strcpy((char *)received_key, rcvkeystring.c_str());
    std::cout << "public key received: " << received_key << std::endl;
}



