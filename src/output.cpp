#include "output.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>



/* writes open ports to console */
void write_open_ports (std::vector<uint32_t> open_ports){
	
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	std::cout << "                  Open Ports					  " << std::endl;
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	
	for (uint32_t port : open_ports) {
		
		std::cout << "Port: " << port << std::endl;
	}
}


/* writes closed ports to console */
void write_closed_ports (std::vector<uint32_t> closed_ports){
	
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	std::cout << "                 Closed Ports					  " << std::endl;
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	
	for (uint32_t port : closed_ports) {
		
		std::cout << "Port: " << port << std::endl;
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
