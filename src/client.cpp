#include "../utils/ipPort.hpp"
#include "../utils/util.hpp"

#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <unistd.h>

static FILE *stream = stdout;

const int MAX_IP_SIZE = 15;
const int MIN_IP_SIZE = 7;
const int MAX_PORT_SIZE = 5;
const int MIN_PORT_SIZE = 4;
const int MAX_PORT = 65535;
const int MIN_PORT = 1024;

void PrintUsage(FILE *f) {
    fprintf(f, "Usage:\n");
    fprintf(f, "        ./client IP:PORT\n");
}

int main(int argc, char *argv[]) {
    util::ErrorMsg *errMsg = new util::ErrorMsg;

    char *ip = nullptr, *port = nullptr;

    if (argc != 2) {
        ip = (char *)malloc(sizeof(char) * (MAX_IP_SIZE + 1));
        memset(ip, '\0', sizeof(ip));
        strcpy(ip, "127.0.0.1");
        port = (char *)malloc(sizeof(char) * (MAX_PORT + 1));
        memset(port, '\0', sizeof(port));
        strcpy(port, "9999");
    } else {
        if (!util::split_IP_Port(argv[1], ip, port)) {
            errMsg->PrintErrorMsg(__FILE__, __func__, __LINE__,
                                  "ip&port is invalid", errno);
            exit(-1);
        }
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        errMsg->PrintErrorMsg(__FILE__, __func__, __LINE__,
                              "create socket error", errno);
        exit(-1);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;

    servaddr.sin_port = htons(atoi(port));
    servaddr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        errMsg->PrintErrorMsg(__FILE__, __func__, __LINE__, "connect error",
                              errno);
        exit(-1);
    }

    char send_msg[] = "GET / HTTP/1.1";
    if (send(sockfd, send_msg, strlen(send_msg), 0) < 0) {
        errMsg->PrintErrorMsg(__FILE__, __func__, __LINE__, "send error",
                              errno);
        exit(-1);
    }

    char recv_msg[1024];
    int recv_n = recv(sockfd, recv_msg, 1024, 0);
    recv_msg[recv_n] = 0;
    printf("recv from server:%s\n", recv_msg);

    close(sockfd);
    if (ip)
        free(ip);
    if (port)
        free(port);

    return 0;
}