//
// Created by Nishanth Prabhu on 31/03/19.
//

#include <vector>
#include <map>
#include "server_info.h"

#ifndef CIS700_PUNG_SLAVE_SERVER_H
#define CIS700_PUNG_SLAVE_SERVER_H

static std::vector<server_info> slaves; // Global vector to hold slave servers information
static std::string slave_name;
static std::map<int, std::string> keys_map;

#endif //CIS700_PUNG_SLAVE_SERVER_H
