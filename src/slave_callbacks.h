//
// Created by Nishanth Prabhu on 31/03/19.
//

#ifndef CIS700_PUNG_SLAVE_CALLBACKS_H
#define CIS700_PUNG_SLAVE_CALLBACKS_H

#define LIBSODIUM_PUBLICKEY_LENGTH 32

void set_and_propagate_client_public_key(int client_id, std::string);
std::string get_public_key(std::string client_id);
bool store_message(std::string label, std::string message);
std::string retrieve_message();

#endif //CIS700_PUNG_SLAVE_CALLBACKS_H
