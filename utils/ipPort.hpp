#ifndef __IP_PORT__
#define __IP_PORT__

#include <iostream>

namespace util {
struct ADDR {
    std::string ip;
    std::string port;
    bool ok;
    ADDR() : ok(false) {}
};

bool split_IP_Port(const char *addr, char *ip, char *port);
ADDR *split_IP_Port(std::string &addr);

} // namespace util

#endif