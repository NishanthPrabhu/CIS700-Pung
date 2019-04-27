#ifndef CIS700_PUNG_CLIENT_H
#define CIS700_PUNG_CLIENT_H

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <iostream>
#include <cstring>
#include <string>
#include <queue>
#include <signal.h>

#include <iostream>
#include <cstdlib>
#include <random>
#include <boost/algorithm/string.hpp>

#include "msg_client.h"
#include "utils/pir_params.h"


using namespace::std;

#define RECEIVE_PORT 40000

enum command
{
	JOIN_PEER,
	MSG,
	QUIT_CHAT,
	QUIT_CLIENT,
	HELP
};

bool run = true;

msg_client client;
msg_peer peer;
string input;
queue<string> msgs;
round_info cur_round;

random_device rd;

static void init();

string get_hex(const unsigned char *, const size_t);

void sigHandler(int sigNum);

void setUpSignals();

string getIPAddress();

void register_client(rpc::client *, int, string, unsigned char*);

void init_new_round(std::string round_id, vector<unsigned char> nonce);

void init_msg_retrieval();

void start_send_msg();

void create_round_labels();

void send_dummy_msg();

void send_msg();

void retrieve_msg();

void initialize_client(int, string, int);

void create_comm_keys(string peer_id);

void display_help();

command get_command();

void add_to_msgqueue();

void remove_peer();

void destroy_keys_and_data();


#endif
