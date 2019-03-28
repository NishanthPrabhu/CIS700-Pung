//
// Created by Nishanth Prabhu on 27/03/19.
//

#include <string>

#ifndef CIS700_PUNG_SERVER_INFO_H
#define CIS700_PUNG_SERVER_INFO_H


class server_info {

	std::string server_name;
	std::string server_ip;
	int port_number;

public:
	server_info(std::string server_name, std::string server_ip, int port);
	std::string get_server_name();
	std::string get_server_ip();
	int get_port();
};


#endif //CIS700_PUNG_SERVER_INFO_H
