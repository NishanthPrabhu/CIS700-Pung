#ifndef CIS700_PUNG_CLIENT_H
#define CIS700_PUNG_CLIENT_H

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <iostream>
#include <cstring>
#include <string>
#include <queue>

#include <iostream>
#include <cstdlib>
#include <boost/algorithm/string.hpp>

#include "SealPIR/pir_client.hpp"
#include "msg_client.h"

using namespace::std;
using namespace seal;

#define MESSAGE_LEN 256
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)
#define RECEIVE_PORT 40000

enum command
{
	JOIN_PEER,
	MSG,
	QUIT_CHAT,
	QUIT_CLIENT,
	HELP
};

msg_client client;
msg_peer peer;
string message;
queue<string> msgs;
round_info cur_round;
bool peer_joined = false;

uint64_t number_of_items = 1 << 12;
uint64_t size_per_item = CIPHERTEXT_LEN;
uint32_t N = 2048;
uint32_t logt = 12;
uint32_t d = 3;
PIRClient *pir_client;
EncryptionParameters *params;
PirParams pir_params;
//GaloisKeys *galois_keys;

static void init();

string get_hex(const unsigned char *, const size_t);

string getIPAddress();

void register_client(rpc::client *, int, string, unsigned char*);

void initialize_new_round(std::string round_id, vector<unsigned char> nonce);

void create_round_labels();

void send_message();

void retrieve_msg();

void initialize_client(int, string, int);

void create_comm_keys(string peer_id);

void display_help();

command get_command();

void add_to_msgqueue();

void destroy_keys_and_data();


#endif
