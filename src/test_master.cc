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
	auto result = client.call("set_client_key", 1, "127.0.0.1:8080", "public_key1");
	string result2 = client.call("get_client_key", "1").as<std::string>();
	
	std::cout << result2 << "\n";
}



