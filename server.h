#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//from Hands on Network Programming with C by Lewis Van Winkle pg 208
void http400(int fd) {
    const char *http400 = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 15\r\n\r\n400 Bad Request";
    send(fd, http400, strlen(http400), 0);
    close(fd);
}
void http404(int fd) {
    const char *http404 = "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: 13\r\n\r\n404 Not Found";
    send(fd, http404, strlen(http404), 0);
    close(fd);
}
void http418(int fd) {
    const char *http418 = "HTTP/1.1 418 I am a teapot\r\nConnection: close\r\nContent-Length: 17\r\n\r\n418 I am a teapot";
    send(fd, http418, strlen(http418), 0);
    close(fd);
}

