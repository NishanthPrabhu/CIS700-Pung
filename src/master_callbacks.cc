//
// Created by Nishanth Prabhu on 28/03/19.
//

#include <iostream>
#include "master_callbacks.h"

bool set_client_public_key(int client_id, std::string key) {
	return true;
}

std::string get_client_public_key(std::string client_id) {
	return "";
}

bool register_client(int client_id) {
	return true;
}

bool store_client_message(std::string message) {
	return true;
}

std::string retrieve_client_message() {
	return "";
}

