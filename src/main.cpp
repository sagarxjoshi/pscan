#include <iostream>
#include <unistd.h>
#include <cstring>
#include <pcap/pcap.h>
#include <libnet.h>
#include <netinet/ether.h>
#include <netinet/tcp.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <sstream>
#include <fstream>


/* custom context structure */
struct ScanContext {

	std::vector<uint32_t> open_ports;
	std::vector<uint32_t> closed_ports;
	
	// thread safety synchronization
	std::mutex mtx;
	std::atomic<bool> is_running{true};
	
};

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

/* packet processing function */
void packet_handler (u_char	*user, const struct pcap_pkthdr *header, const u_char *packet){
	
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


void write_open_ports (std::vector<uint32_t> open_ports){
	
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	std::cout << "                  Open Ports					  " << std::endl;
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	
	for (uint32_t port : open_ports) {
		
		std::cout << "Port: " << port << std::endl;
	}
}

void write_closed_ports (std::vector<uint32_t> closed_ports){
	
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	std::cout << "                 Closed Ports					  " << std::endl;
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	
	for (uint32_t port : closed_ports) {
		
		std::cout << "Port: " << port << std::endl;
	}
}


void open_port_file(std::vector<uint32_t> open_ports, std::string file){
	
	std::ofstream out_file(file);
	
	if (out_file.is_open()) {
		
		out_file << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		out_file << "                  Open Ports					  " << std::endl;
		out_file << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		
		for (auto port : open_ports){
			
			out_file << "Ports: " << port << std::endl;
		}
		
		out_file.close();
		
	}else{
		
		std::cerr << "Error opening file: " << file << std::endl;
	}
}


void closed_port_file(std::vector<uint32_t> closed_ports, std::string file){
	
	std::ofstream out_file(file);
	
	if (out_file.is_open()) {
		
		out_file << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		out_file << "                  Closed Ports					  " << std::endl;
		out_file << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		
		for (auto port : closed_ports){
			
			out_file << "Ports: " << port << std::endl;
		}
		
		out_file.close();
	}else{
		
		std::cerr << "Error opening file: " << file << std::endl;
	}
}

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