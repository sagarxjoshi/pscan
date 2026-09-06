#include <iostream>
#include <unistd.h>
#include <string>
#include <cmath>
#include <cstdint>
#include <string_view>
#include <ranges>
#include <vector>
#include <sstream>
#include "cidr.h"


/* converts integer IP octets to printable string */
std::string get_ipstr(std::size_t ipaddr_int){


	std::vector<std::string> ipaddr_octet = {
		std::to_string((ipaddr_int >> 24)), 
		std::to_string((ipaddr_int >> 16) & 255), 
		std::to_string((ipaddr_int >> 8) & 255), 
		std::to_string((ipaddr_int & 255))

	};


	/* join octets using . delimiter */
	auto joined_ipaddr = ipaddr_octet | std::views::join_with('.');

	/* convert to printable IP string */
	std::string ipaddr_str(joined_ipaddr.begin(), joined_ipaddr.end());
	return ipaddr_str;

}

/* converts IP string to an unsigned integer */
std::size_t get_ipint(const std::string& ipaddr_str){
	
	std::size_t oct1, oct2, oct3, oct4;
	char dot;
	
	std::stringstream ss(ipaddr_str);
	
	/* convert each octet to uint while discarding "." */
	ss >> oct1 >> dot >> oct2 >> dot >> oct3 >> dot >> oct4;
	
	return ((oct1 << 24) | (oct2 << 16)  | (oct3 << 8) | oct4);
}


/* converts cidr to IP range - returns network structure */
network cidr_toipaddr(std::string cidr_block){


	std::size_t max_hosts;
	std::size_t host_bits;
	std::size_t mask_bits;
	std::string ipaddr_str;
	std::size_t ipaddr_int;
	std::size_t net_addr;
	std::size_t broadcast_addr;
	std::size_t mask;
	int shift = 24;
	std::size_t bitwiseor_ipaddr = 0;
	network net;
	
	/* separate out the mask bits */
	std::size_t char_pos = cidr_block.find("/");
	if(char_pos != std::string::npos){

		/* calculating host bits */
		mask_bits = std::stoi(cidr_block.substr(char_pos + 1));
		host_bits = 32 - mask_bits;
		
		/* identifying max hosts */
		max_hosts = std::pow(2, host_bits);
		net.hosts = max_hosts;

		
		/* separate out the IP string */
		ipaddr_str = cidr_block.substr(0, char_pos);
	
		/* converting IP string octets to unsigned integer */	
		std::string_view delim = ".";
		auto split_view = ipaddr_str | std::views::split(delim);
		std::vector<std::string> octet;

		for (auto part : split_view){

			octet.emplace_back(part.begin(), part.end());

		}


		for (auto octet_bit : octet) {
			
			ipaddr_int = static_cast<uint32_t>(std::stoul(octet_bit));
			/* left shift operation on each octet, shift values changes by -8 */
			ipaddr_int = ipaddr_int << shift;
			/* bitwise OR on each integer octet to produce resultant IP unsigned integer value */
			bitwiseor_ipaddr |= ipaddr_int;
			shift = shift - 8;

		}
		

	}else{

		std::cerr << "Error: Incorrect CIDR range." << std::endl;

	}
	
	/* generating subnet mask:
 	   performing bitwise left shift operation
	   on all 0xffffffff which represents all bits set
	*/
	mask = 0xffffffff << host_bits;
	net.subnet_mask = get_ipstr(mask);
	
	/* generating the network address (start IP) */
	net_addr = bitwiseor_ipaddr & mask;

	net.network_ip = get_ipstr(net_addr);


	/* generating the broadcast address (end IP)
 	   flip the mask bits and perform bitwise OR against network address.
 
 	   note: perform bitwise AND against 0xffffffff to drop top 32-bits
	   since we are performing operation on 32-bit value
	*/
	broadcast_addr = net_addr | ((~mask) & 0xffffffff);	
	net.broadcast_ip = get_ipstr(broadcast_addr);

	return net;

}

