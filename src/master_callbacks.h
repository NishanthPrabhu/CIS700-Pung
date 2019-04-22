//
// Created by Nishanth Prabhu on 28/03/19.
//
#include <map>
#include "rpc/msgpack.hpp"

#ifndef CIS700_PUNG_MASTER_CALLBACKS_H
#define CIS700_PUNG_MASTER_CALLBACKS_H

#define LIBSODIUM_PUBLICKEY_LENGTH 32

extern std::map<int, std::string> client_address_map;
extern std::vector<std::tuple<std::string, int>> label_map;
bool set_client_public_key(int client_id, std::string const& client_ip,
                           std::vector<unsigned char> const&, std::string const& galoiskey);
std::vector<unsigned char> get_client_public_key(int client_id);
void store_client_message(std::string label, std::vector<unsigned char> const& message);
std::string retrieve_message(int client_id, std::vector<std::string> serializedQuery);
std::vector<std::tuple<std::string, int>> get_label_mapping();
void test_unsigned_char(std::vector<unsigned char> buff);
//void test_unsigned_char(RPCLIB_MSGPACK::sbuffer buff);

#endif //CIS700_PUNG_MASTER_CALLBACKS_H
