#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "utils/server_info.h"
#include "slave_callbacks.h"
#include "rpc/server.h"
#include <boost/algorithm/string.hpp>

std::vector<server_info> slaves;
std::string slave_name;

// parses server_list file and returns the slave and master info
server_info* get_slave_server(std::string server_file, std::string server_name) {
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

			if (tokens[0].compare("master") == 0)
				continue;

			if (tokens[0].compare(server_name) != 0) {
				server_info client(tokens[0], tokens[1], std::stoi(tokens[2]));
				slaves.push_back(client);
			  	continue;
      		}

            std::cout << tokens[0] << std::endl;
			info = new server_info(tokens[0], tokens[1], std::stoi(tokens[2]));
		}
		filereader.close();
	} else {
		std::cerr << "Unable to open server file. Fatal error.";
		exit(1);
	}

	return info;
}

// sets up all rpc callbacks and starts the slave server 
void start_slave_server(server_info *info) {
	//initialize_pir();
    int port = info->get_port();
	initialize_pir();
    rpc::server srv(port);

    srv.bind("set_client_key", &set_client_public_key);
	srv.bind("set_and_propagate_client_key", &set_and_propagate_client_public_key); // Client sends it's public key
	srv.bind("get_client_key", &get_public_key); // Client receives another's public key
	srv.bind("store_and_propagate_message", &store_and_propagate_message); // Client sends message to store
	srv.bind("store_message", &store_message);
    srv.bind("retrieve_client_message", &retrieve_message); // Client retrieves message
	srv.bind("rounds_notice", &initialize_new_round);
    srv.bind("index_vote", &send_index_vote);
    srv.bind("client_shutdown", &shutdown_client);
    srv.bind("retrieve_notice", &start_retrieve_stage);
    srv.run(); // Change this to async?
    //std::cin.ignore();
}

int main(int argc, char **argv) {
	if (argc < 3) {
		std::cerr << "Usage: slave_server [server_list_file] [slave{slave id}]" << std::endl;
		exit(1);
	}

	std::string server_file = argv[1];
	std::string server_name = argv[2];
	server_info* info = get_slave_server(server_file, server_name);
	slave_name = server_name;
	start_slave_server(info);
}
