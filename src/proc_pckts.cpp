#include "capture.h"
#include "proc_pckts.h"
#include "write_pckts.h"
#include <netinet/ether.h>
#include <netinet/tcp.h>


/* packet processing function */
void packet_handler (u_char *user, const struct pcap_pkthdr *header, const u_char *packet){
	
	auto *context = reinterpret_cast<ScanContext*>(user);
	struct tcphdr *tcp = (struct tcphdr *) (packet + LIBNET_IPV4_H + LIBNET_ETH_H);
	
	// thread safe vector write operation
	std::lock_guard<std::mutex> lock(context->mtx);
	
	if (tcp->th_flags == 0x14){
		
		context->closed_ports.push_back(ntohs(tcp->th_sport));

	}else{
		
		if (tcp->th_flags == 0x12){
			
			context->open_ports.push_back(ntohs(tcp->th_sport));

		}
	}

}


/* wrapper for packet processing */
void run_receiver(pcap_t *pcap_handle, ScanContext *context){
	
	while (context->is_running){
		
		pcap_dispatch(pcap_handle, -1, packet_handler, reinterpret_cast<u_char*>(context));
	
	}
}

