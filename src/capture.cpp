#include <iostream>
#include "capture.h"


/*	
	selecting interface for packet capture 
	first element is used as interface name: alldevsp->name 
*/
std::string get_interface() {
	
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if_t *alldevsp;
	
	if (pcap_findalldevs(&alldevsp, errbuf) == -1){
		
		std::cerr << "Error while trying to look for an interface: " << errbuf << std::endl;
		std::exit(EXIT_FAILURE);
	}else if (alldevsp == NULL) {
		
		std::cout << "No interfaces found: " << errbuf << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
	return alldevsp->name;
}

/* initializes packet capture */
pcap_t* init_pcap(std::string interface, uint32_t src_port) {
	
	char errbuf[PCAP_ERRBUF_SIZE];
	
	/* opening a network interface for capture */
	pcap_t *handle = pcap_open_live(interface.c_str(), BUFSIZ, -1, 1000, errbuf);
	if (handle == NULL) {
		
		std::cerr << "coulf not open device: " << errbuf << std::endl;
	}
	
	/* set up filtering for incoming packets */
	struct bpf_program fp;
	std::string filter_exp = "tcp and dst port " + std::to_string(src_port);
	
	if(pcap_compile(handle, &fp, filter_exp.c_str(), 0, PCAP_NETMASK_UNKNOWN) == -1 || pcap_setfilter(handle, &fp) == -1 ){
		
		std::cerr << "Error in setting filter: " << pcap_geterr(handle) << std::endl;
	}
	
	/* free memory used for filtering */
	pcap_freecode(&fp);
	return handle;
}
