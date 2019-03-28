//
// Created by Nishanth Prabhu on 28/03/19.
//

#include <vector>
#include "server_info.h"

#ifndef CIS700_PUNG_MASTER_SERVER_H
#define CIS700_PUNG_MASTER_SERVER_H

static std::vector<server_info> slaves; // Global vector to hold slave servers information
static int load_balancer_index;
int get_slave_index(); // Round robin through the slave vector and choose

#endif //CIS700_PUNG_MASTER_SERVER_H
