#include "ipPort.hpp"
#include <stdlib.h>
#include <string.h>

namespace util {

bool split_IP_Port(const char *addr, char *ip, char *port) {
    size_t addrLen = strlen(addr);
    size_t ipLen = 0, portLen = 0;
    bool failed = true;
    for (size_t i = 0; i < addrLen; i++) {
        if (addr[i] == ':') {
            ipLen = i;
            portLen = addrLen - ipLen - 1;
            failed = false;
            break;
        }
    }
    if (failed)
        return false;

    if (ip == nullptr) {
        // logs.warning("you shoule locallly initialize ip & port")
        ip = strndup(addr, ipLen);
    } else {
        size_t len = strlen(ip);
        if (len < ipLen)
            return false;
        strncpy(ip, addr, ipLen);
        ip[ipLen] = '\0';
    }

    if (port == nullptr) {
        // logs.warning("you shoule locallly initialize ip & port")
        port = strndup(addr + ipLen + 1, portLen);
    } else {
        size_t len = strlen(port);
        if (len < portLen)
            return false;
        strncpy(port, addr + ipLen + 1, portLen);
        port[portLen] = '\0';
    }
    return true;
}

ADDR *split_IP_Port(std::string &addr) {
    std::size_t addrLen = addr.size();
    ADDR *ret = new (ADDR);

    for (std::size_t i = 0; i < addrLen; i++) {
        if (addr[i] == ':') {
            ret->ok = true;
            ret->ip = addr.substr(0, i);
            ret->port = addr.substr(i + 1);
            return ret;
        }
    }
    return ret;
}

} // namespace util