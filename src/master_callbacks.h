//
// Created by Nishanth Prabhu on 28/03/19.
//

#ifndef CIS700_PUNG_MASTER_CALLBACKS_H
#define CIS700_PUNG_MASTER_CALLBACKS_H

#define LIBSODIUM_PUBLICKEY_LENGTH 32

bool set_client_public_key(int client_id, std::string);
std::string get_client_public_key(std::string client_id);
bool register_client(int client_id);
bool store_client_message(std::string message);
std::string retrieve_client_message();

#endif //CIS700_PUNG_MASTER_CALLBACKS_H
