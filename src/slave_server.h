//
// Created by Nishanth Prabhu on 31/03/19.
//

#include <vector>
#include <map>
#include "server_info.h"
#include "SealPIR/pir.hpp"
#include "SealPIR/pir_client.hpp"
#include "SealPIR/pir_server.hpp"
#include <seal/seal.h>
#include <sodium.h>

#ifndef CIS700_PUNG_SLAVE_SERVER_H
#define CIS700_PUNG_SLAVE_SERVER_H

extern std::vector<server_info> slaves; // Global vector to hold slave servers information
extern std::string slave_name;

#define MESSAGE_LEN 256
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)

uint64_t number_of_items = 1 << 12;
uint64_t size_per_item = CIPHERTEXT_LEN;
uint32_t N = 2048;
uint32_t logt = 12;
uint32_t d = 3;

#endif //CIS700_PUNG_SLAVE_SERVER_H
