#pragma once

#include <iostream>
#include <string>


/* network structure defining IP, hosts and mask */
struct network{

        std::string subnet_mask;
        std::string network_ip;
        std::string broadcast_ip;
        std::size_t hosts;

};

std::string get_ipstr(std::size_t ipaddr_int);

std::size_t get_ipint(const std::string& ipaddr_str);

network cidr_toipaddr(std::string cidr_block);


