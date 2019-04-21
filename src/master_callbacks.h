//
// Created by Nishanth Prabhu on 28/03/19.
//
#include <map>
#include "bloom_filter.hpp"
#include "rpc/msgpack.hpp"

#ifndef CIS700_PUNG_MASTER_CALLBACKS_H
#define CIS700_PUNG_MASTER_CALLBACKS_H

#define LIBSODIUM_PUBLICKEY_LENGTH 32

struct mapping {
    std::vector<std::tuple<std::string, int>> label_map;
    MSGPACK_DEFINE(label_map);
};

extern std::map<int, std::string> client_address_map;
bool set_client_public_key(int client_id, std::string const& client_ip, std::vector<unsigned char> const&);
std::string get_client_public_key(int client_id);
void store_client_message(std::string label, std::string message);
std::string retrieve_client_message();
mapping get_label_mapping();
void test_unsigned_char(std::vector<unsigned char> buff);
//void test_unsigned_char(RPCLIB_MSGPACK::sbuffer buff);

#endif //CIS700_PUNG_MASTER_CALLBACKS_H
