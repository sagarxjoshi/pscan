#include "output.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>



/* writes open ports to console */
void write_open_ports (std::vector<uint32_t>& open_ports, std::ostream& os){
	
	os << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	os << "                  Open Ports					  " << std::endl;
	os << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	
	for (const auto& port : open_ports) {
		
		os << "Port: " << port << std::endl;
	}
}


/* writes closed ports to console */
void write_closed_ports (std::vector<uint32_t>& closed_ports, std::ostream& os){
	
	os << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	os << "                 Closed Ports					  " << std::endl;
	os << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	
	for (const auto& port : closed_ports) {
		
		os << "Port: " << port << std::endl;
	}
}


/* writes open ports to an output file */
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


/* writes closed ports to an ouput file */
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
