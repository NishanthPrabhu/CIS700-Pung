//
// Created by Nishanth Prabhu on 31/03/19.
//

#ifndef CIS700_PUNG_PIR_PARAMS_H
#define CIS700_PUNG_PIR_PARAMS_H

#include "SealPIR/pir.hpp"
#include <seal/seal.h>
#include <sodium.h>

using namespace seal;

#define MESSAGE_LEN 256
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)
#define NUM_PIR_REPLY_CIPHER 10

uint64_t number_of_items = 1 << 12;
uint64_t size_per_item = CIPHERTEXT_LEN;
uint32_t N = 2048;
uint32_t logt = 12;
uint32_t d = 2;

#endif //CIS700_PUNG_PIR_PARAMS_H
