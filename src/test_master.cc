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
	auto result = client.call("set_client_key", 1, "publickey1");
}



