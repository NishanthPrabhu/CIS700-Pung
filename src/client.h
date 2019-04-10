#ifndef CIS700_PUNG_CLIENT_H
#define CIS700_PUNG_CLIENT_H

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <iostream>
#include <cstring>
#include <string>

#include <iostream>
#include <cstdlib>
#include <boost/algorithm/string.hpp>

#include "msg_client.h"

using namespace::std;

static void init();

string get_hex(const unsigned char *, const size_t);

string getIPAddress();

void register_client(rpc::client *, int, string, unsigned char*);

void initialize_client(msg_client &, int, string, int);

void create_comm_keys(msg_client &client, msg_peer &peer, int peer_id);


#endif
