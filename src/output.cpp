#include "output.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>



/* writes open ports to console/file */
void write_open_ports (std::vector<uint32_t>& open_ports, std::ostream& os){
	
	os << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	os << "                  Open Ports					  " << std::endl;
	os << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	
	for (const auto& port : open_ports) {
		
		os << "Port: " << port << std::endl;
	}
}


/* writes closed ports to console/file */
void write_closed_ports (std::vector<uint32_t>& closed_ports, std::ostream& os){
	
	os << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	os << "                 Closed Ports					  " << std::endl;
	os << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	
	for (const auto& port : closed_ports) {
		
		os << "Port: " << port << std::endl;
	}
}


void progress_bar (std::size_t percentage, std::size_t max_hosts) {
	
	std::string progress = "[" + std::string(percentage, '*') + std::string(max_hosts - percentage, ' ') + "]";
	std::cout << progress << "\r\033[F" << std::flush;
}