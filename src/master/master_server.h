//
// Created by Nishanth Prabhu on 28/03/19.
//

#ifndef CIS700_PUNG_MASTER_SERVER_H
#define CIS700_PUNG_MASTER_SERVER_H

#include <vector>
#include "utils/server_info.h"

extern std::vector<server_info> slaves; // Global vector to hold slave servers information
static int load_balancer_index;
int get_slave_index(); // Round robin through the slave vector and choose
int get_label_index();

#endif //CIS700_PUNG_MASTER_SERVER_H
