#include <iostream>
#include <unistd.h>
#include <cstring>
#include <libnet.h>
#include <string>
#include <thread>
#include <sstream>
#include "output.h"
#include "capture.h"
#include "write_pckts.h"
#include "proc_pckts.h"

 
int main(int argc, char *argv[]) {
	
	/* check program's calling arguments */
	if (argc < 2) {
		
		std::cerr << "Insufficient arguments, for usage: " << argv[0] << " -h or --help" << std::endl;
		return 1;
	}
	
	std::string target;
	std::string portRange;
	std::string token;
	std::vector <std::string> ports;
	std::string outfile;
	
	/* defining program options */
	for ( int i = 1; i < argc; i++ ){
		
		std::string arg = argv[i];
		
		if ( (arg == "-t" || arg == "--target") && i + 1 < argc ){
			
			/* assign next value as argument and increment index */
			target = argv[++i];
			
		}else if ( (arg == "-p" || arg == "--ports") && i + 1 < argc ) {
			
			/* stores the port range user defines */
			portRange = argv[++i];
			std::istringstream stream(portRange);
			
			/* storing ports in a vector structure to use later*/
			while (std::getline(stream, token, '-')) {
				
				ports.push_back(token);
			}			
		}else if ( (arg == "-o" || arg == "--out-file") && i + 1 < argc ){
			
			outfile = argv[++i];
			
		}else if ( (arg == "-h" || arg == "--help") ) {
			
			std::cout << " -t or --target <IP address> " << std::endl;
			std::cout << " -p or --ports <start port-end port> " << std::endl;
			std::cout << " -o or --out-file <ouput file name> " << std::endl;
			std::cout << " -h or --help <help menu> " << std::endl;
			return 0;

		}else{
			
			std::cerr << "Unknown or invalid options: " << arg << std::endl;
			return 1;
		}
	};
	
	uint32_t src_port = 4444;
	uint32_t start_port = std::stoi(ports.at(0));
	uint32_t end_port = std::stoi(ports.at(1));
	uint32_t src_ip, dst_ip;
	libnet_t *l;
	pcap_t *handle;
	std::string device;
	
	
	
	/* set interface */
	device = get_interface();
	
	/* initialize packet capture */
	handle = init_pcap(device, src_port);
	
	/* initialize libnet context */
	l = init_libnet(device);
	if (!l) {
		
		pcap_close(handle);
		return 1;
	}
	
	/* assigning IP addresses */
	src_ip = libnet_get_ipaddr4(l);
	if (src_ip < 0){
		
		std::cerr << "Failed to get source IP address: " << libnet_geterror(l) << std::endl;
		return 1;
	}
	
	dst_ip = libnet_name2addr4(l, target.c_str(), LIBNET_RESOLVE);
	if (dst_ip < 0){
		
		std::cerr << "Bad destination IP address: " << libnet_geterror(l) << std::endl;
		return 1;
	}
	

	
	ScanContext context;
	
	std::thread receiver(run_receiver, handle, &context);
	/* scan loop */
	for (uint32_t port = start_port; port <= end_port; port++){
		
		send_syn(l, src_port, port, src_ip, dst_ip);
		
		std::this_thread::sleep_for(std::chrono::microseconds(400));
	}
	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	
	context.is_running = false;
	pcap_breakloop(handle);
	receiver.join();
	
	if (argv[3]) {
		
		/* write output to file */
		std::string open_file = outfile + "_open";
		std::string closed_file = outfile + "_closed";
		
		open_port_file(context.open_ports, open_file);
		closed_port_file(context.closed_ports, closed_file);
		
	}else{
		
		/* print resulting open and closed ports to console */
		write_open_ports(context.open_ports);
		std::cout << std::endl;
		write_closed_ports(context.closed_ports);
	}

	
	pcap_close(handle);
	libnet_destroy(l);


}