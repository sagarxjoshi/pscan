#pragma once

#include <libnet.h>
#include <string>
#include <cstdint>
#include <iostream>

libnet_t* init_libnet(std::string interface);
bool send_syn(libnet_t *lcontext, uint32_t src_port, uint32_t dst_port, uint32_t src_ip, uint32_t dst_ip);

