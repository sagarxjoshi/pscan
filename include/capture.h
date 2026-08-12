#pragma once

#include <pcap/pcap.h>
#include <string>
#include <cstdint>

std::string get_interface();
pcap_t* init_pcap(std::string interface, uint32_t src_port);

