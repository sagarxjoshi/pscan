#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

void write_open_ports (std::vector<uint32_t>& open_ports, std::ostream& os);
void write_closed_ports (std::vector<uint32_t>& closed_ports, std::ostream& os);

void open_port_file (std::vector<uint32_t> open_ports, std::string file);
void closed_port_file (std::vector<uint32_t> closed_ports, std::string file);


