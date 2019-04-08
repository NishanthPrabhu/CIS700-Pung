//
// Created by Nishanth Prabhu on 27/03/19.
//

#include <string>

#ifndef CIS700_PUNG_CLIENT_INFO_H
#define CIS700_PUNG_CLIENT_INFO_H


class client_info {

	std::string client_ip;
	std::string publickey;
	int client_id;

public:
    client_info(int client_id, std::string const& client_ip, std::string const& publickey);
	int get_client_id();
	std::string get_client_ip();
    std::string get_public_key();
};


#endif //CIS700_PUNG_CLIENT_INFO_H
