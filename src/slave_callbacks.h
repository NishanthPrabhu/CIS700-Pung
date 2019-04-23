//
// Created by Nishanth Prabhu on 31/03/19.
//

#ifndef CIS700_PUNG_SLAVE_CALLBACKS_H
#define CIS700_PUNG_SLAVE_CALLBACKS_H

#include<vector>

#define LIBSODIUM_PUBLICKEY_LENGTH 32

void initialize_new_round(std::string round_id);
void set_client_public_key(int client_id, std::string const& client_ip,
                           std::vector<unsigned char> const&, std::string const&);
void set_and_propagate_client_public_key(int client_id, std::string const& client_ip,
                        std::vector<unsigned char> const &publickey, std::string const& galoiskey);
std::vector<unsigned char> get_public_key(int client_id);
void store_and_propagate_message(int index, std::vector<unsigned char> const& label,
                                 std::vector<unsigned char> const& message);
void store_message(int index, std::vector<unsigned char> const& label, std::vector<unsigned char> const& message);
std::string retrieve_message(int, std::vector<std::string>);
int send_index_vote();

#endif //CIS700_PUNG_SLAVE_CALLBACKS_H
