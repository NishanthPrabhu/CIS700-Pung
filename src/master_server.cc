#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "server_info.h"
#include "master_callbacks.h"
#include "rpc/server.h"
#include "rpc/client.h"
#include "master_server.h"
#include <boost/algorithm/string.hpp>

std::vector<server_info> slaves;

server_info* get_master_server(std::string server_file) {
	std::ifstream filereader(server_file);
	std::string line;
  	server_info *info = NULL;

	if (filereader.is_open()) {
		while (std::getline(filereader, line)) {
			std::stringstream ss(line);
			std::string token;
			std::vector<std::string> tokens;
			while (std::getline(ss, token, ',')) {
				tokens.push_back(token);
				boost::trim(token);
			}

			if (tokens[0].compare("master")) {
				server_info client(tokens[0], tokens[1], std::stoi(tokens[2]));
                std::cout<<"Adding slave: " + tokens[0] << std::endl;
                slaves.push_back(client);
			  continue;
      		}

			info = new server_info(tokens[0], tokens[1], std::stoi(tokens[2]));
		}
		filereader.close();
	} else {
		std::cerr << "Unable to open server file. Fatal error.";
		exit(1);
	}

	return info;
}

int get_slave_index() {
	int slave_index;
    slave_index = load_balancer_index;
	auto size = slaves.size();
	load_balancer_index = (load_balancer_index + 1) % (int)size;
    std::cout << "Choosing slave: " << slave_index << std::endl;
    return slave_index;
}

void start_master_server(server_info *info) {
	int port = info->get_port();
	rpc::server srv(port);

	srv.bind("set_client_key", &set_client_public_key); // Client sends it's public key
	srv.bind("get_client_key", &get_client_public_key); // Client receives another's public key
	srv.bind("store_message", &store_client_message); // Client sends message to store
	srv.bind("retrieve_message", &retrieve_client_message); // Client retrieves message
    srv.async_run(3);
    std::cin.ignore();
}

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "Usage: master_server [server_list_file]";
		exit(1);
	}

	load_balancer_index = 0;
	std::string server_file = argv[1];
	server_info* info = get_master_server(server_file);
	start_master_server(info);
}
