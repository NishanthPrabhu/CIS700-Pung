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
    std::vector<unsigned char> publickey;
    std::string galoiskey;
    int client_id;

public:
	client_info();
    client_info(int client_id, std::string const& client_ip,
                std::vector<unsigned char> const& publickey, std::string const& galoiskey);
	int get_client_id();
	std::string get_client_ip();
    std::vector<unsigned char> get_public_key();
    std::string get_galois_key();
};


#endif //CIS700_PUNG_CLIENT_INFO_H
