#pragma once

#include <pcap/pcap.h>
#include <cstdint>
#include <libnet.h>
#include <iostream>
#include <atomic>
#include <vector>
#include <mutex>

/* custom context structure */
struct ScanContext {

	std::vector<uint32_t> open_ports;
	std::vector<uint32_t> closed_ports;
	
	// thread safety synchronization
	std::mutex mtx;
	std::atomic<bool> is_running{true};
	
};


void packet_handler (u_char *user, const struct pcap_pkthdr *header, const u_char *packet);
void run_receiver(pcap_t *pcap_handle, ScanContext *context);


