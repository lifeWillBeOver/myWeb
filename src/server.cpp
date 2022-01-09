#include "../utils/util.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <unistd.h>

const int MAX_RECV = 1024;

static void returnHTML(int connfd, int statusCode, const char *filename) {
    if (statusCode == 404) {
        char ret[] = "HTTP/1.1 404 Not Found\r\n"
                     "Content-Type: text/plain; charset=utf-8\r\n"
                     "Date: Mon, 04 Oct 2021 20:20:08 GMT\r\n"
                     "Content-Length: 19\r\n\r\n"
                     "404 page not found\r\n";
        send(connfd, ret, strlen(ret), 0);
    } else if (statusCode == 200) {
        char ret[] = "HTTP/1.1 200 OK\r\n"
                     "Date: Mon, 04 Oct 2021 20:20:12 GMT\r\n"
                     "Content-Length: 5\r\n"
                     "Content-Type: text/plain; charset=utf-8\r\n\r\n"
                     "Hello\r\n";
        send(connfd, ret, strlen(ret), 0);
    }
}

int main(int argc, char *argv[]) {
    util::ErrorMsg *errMsg = new util::ErrorMsg;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        errMsg->PrintErrorMsg(__FILE__, __func__, __LINE__,
                              "create socket error", errno);
        exit(-1);
    }

    // not to wait TIME_WAIT
    int on = 1;
    if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
        errMsg->PrintErrorMsg(__FILE__, __func__, __LINE__, "setsockopt failed",
                              errno);
        exit(-1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(9999);

    if ((bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) ==
        -1) {
        errMsg->PrintErrorMsg(__FILE__, __func__, __LINE__, "bind error",
                              errno);
        exit(-1);
    }

    if (listen(sockfd, 3) == -1) {
        errMsg->PrintErrorMsg(__FILE__, __func__, __LINE__, "listen error",
                              errno);
        exit(-1);
    }

    /*
        GET / HTTP/1.1
        Host: localhost:9999
        User-Agent: curl/7.64.0
    */
    //    Accept: */*

    int connfd = 0;
    char buff[MAX_RECV];
    char path[MAX_RECV];

    while (true) {
        if ((connfd = accept(sockfd, (struct sockaddr *)NULL, NULL)) == -1) {
            errMsg->PrintErrorMsg(__FILE__, __func__, __LINE__, "accept error",
                                  errno);
            continue;
        }
        memset(path, '\0', sizeof(path));
        while (true) {
            int n = recv(connfd, buff, MAX_RECV, 0);
            if (n == -1) {
                break;
            }
            buff[n] = '\0';
            if (strncasecmp("GET", buff, 3) == 0) {
                int len = strlen(buff);
                int idx1 = 4, idx2 = 0;
                while (buff[idx1] != ' ') {
                    path[idx2++] = buff[idx1++];
                }
                if (strcmp(path, "/") == 0) {
                    returnHTML(connfd, 200, nullptr);
                } else {
                    returnHTML(connfd, 404, nullptr);
                }
            }
        }
        close(connfd);
    }
    close(sockfd);
}