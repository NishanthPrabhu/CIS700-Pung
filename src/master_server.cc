#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include "server_info.h"
#include "master_callbacks.h"
#include "rpc/server.h"
#include "rpc/client.h"
#include "rpc/rpc_error.h"
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

void send_rounds_notice_clients(int round_number) {
    std::string round_id = "round_" + std::to_string(round_number);
    
    for (auto const& client : client_address_map) {
        int client_id = client.first;
        std::string client_ip_port = client.second;
        std::vector<std::string> ip_port;
        boost::split(ip_port, client_ip_port, boost::is_any_of(":"));
        rpc::client rpcclient(ip_port[0], stoi(ip_port[1]));

        try {
            const uint64_t short_timeout = 1000;
            rpcclient.set_timeout(short_timeout);
            rpcclient.call("rounds_notice", round_id);
        } catch (rpc::timeout &t) {
            std::cout << "Client not responding..skip" << std::endl;
            continue;
        }
    }    
}    


void send_rounds_notice_slaves(int round_number) {
    std::string round_id = "round_" + std::to_string(round_number);
    
    for (int i = 0; i < slaves.size(); i++) {
        server_info slave = slaves[i];
        std::string slave_ip = slave.get_server_ip();
        int port = slave.get_port();
        rpc::client client(slave_ip, port);

        try {
            const uint64_t short_timeout = 1000;
            client.set_timeout(short_timeout);
            client.call("rounds_notice", round_id);
        } catch (rpc::timeout &t) {
            std::cout << "Slave not responding..skip" << std::endl;
            continue;
        }
    }    
}    

void round_master(int round_length) {
    int round_number = 0;

    while (true) {
        round_number += 1;
        
        // Tell the slaves first, so that they can clear state.
        send_rounds_notice_slaves(round_number);

        // Tell all clients (So master needs to maintain a list of clients and their information)
        send_rounds_notice_clients(round_number);

        std::this_thread::sleep_for(std::chrono::milliseconds(round_length));
    }    



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
	if (argc < 3) {
		std::cerr << "Usage: master_server [server_list_file] [length of round(in milliseconds)]";
		exit(1);
	}

	load_balancer_index = 0;
	std::string server_file = argv[1];
    int round_length = std::stoi(argv[2]);
	server_info* info = get_master_server(server_file);
    //std::thread thread(&round_master, std::ref(round_length));
    start_master_server(info);
}
