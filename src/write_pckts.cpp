#include "write_pckts.h"
#include <pcap/pcap.h>

/* initialize libnet */
libnet_t* init_libnet(std::string interface) {
	
	char errbuf[PCAP_ERRBUF_SIZE];
	
	/* setting up libnet context */
	libnet_t *lcontext;
	lcontext = libnet_init(LIBNET_RAW4, interface.c_str(), errbuf);
	
	return lcontext;
		
}


/* Sends packets */
bool send_syn(libnet_t *lcontext, uint32_t src_port, uint32_t dst_port, uint32_t src_ip, uint32_t dst_ip) {
	
	/* clearing packet buffer */
	libnet_clear_packet(lcontext);
	
	/* building TCP header */
	libnet_ptag_t tcp=0;
	tcp = libnet_build_tcp(
		src_port, dst_port, libnet_get_prand(LIBNET_PRu16),
		0, TH_SYN, 7, 0, 0, LIBNET_TCP_H, NULL, 0, lcontext, tcp
		);
	
	if(tcp == -1){
		std::cerr << "Failed to build TCP header: " << libnet_geterror(lcontext) << std::endl;
		return false;
	}
	
	
	/* building IPv4 header */
	libnet_ptag_t ipv4=0;
	ipv4 = libnet_build_ipv4(
		LIBNET_TCP_H+LIBNET_IPV4_H, 0, libnet_get_prand(LIBNET_PRu16),
		0, 127, IPPROTO_TCP, 0, src_ip, dst_ip, NULL, 0, lcontext, ipv4
		);
	
	if(ipv4 == -1){
		
		std::cerr << "Failed to build IP header: " << libnet_geterror(lcontext) << std::endl;
		return false;
	}
	
	/* write packets */
	if(libnet_write(lcontext) == -1){
		
		std::cerr << "Failed to send packet: " << libnet_geterror(lcontext) << std::endl;
		return false;
	}
	
	return true;
}
