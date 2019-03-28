#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "server_info.h"
#include <boost/algorithm/string.hpp>

std::vector<server_info> slaves; // Global vector to hold slave servers information

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

void start_master_server() {


}

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "Usage: master_server [server_list_file]";
		exit(1);
	}

	std::string server_file = argv[1];
	server_info* info = get_master_server(server_file);
	start_master_server();
}
