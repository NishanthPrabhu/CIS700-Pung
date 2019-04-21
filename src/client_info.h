//
// Created by Nishanth Prabhu on 27/03/19.
//

#ifndef CIS700_PUNG_CLIENT_INFO_H
#define CIS700_PUNG_CLIENT_INFO_H

#include <string>
#include <vector>

using namespace std;

class client_info {

	std::string client_ip;
	//std::string publickey;
    std::vector<unsigned char> publickey;
    int client_id;

public:
	client_info();
    client_info(int client_id, std::string const& client_ip, std::vector<unsigned char> const& publickey);
	int get_client_id();
	std::string get_client_ip();
    std::vector<unsigned char> get_public_key();
};


#endif //CIS700_PUNG_CLIENT_INFO_H
