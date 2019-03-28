//
// Created by Nishanth Prabhu on 27/03/19.
//

#include "server_info.h"

server_info::server_info(std::string server_name, std::string server_ip, int port) {
	this->server_name = server_name;
	this->server_ip = server_ip;
	port_number = port;
}

int server_info::get_port() {
	return port_number;
}

std::string server_info::get_server_name() {
	return server_name;
}

std::string server_info::get_server_ip() {
	return server_ip;
}