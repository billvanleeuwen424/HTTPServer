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
    const char *http400 = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 11\r\n\r\nBad Request";
    send(fd, http400, strlen(http400), 0);
    close(fd);
}
void http404(int fd) {
    const char *http400 = "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: 9\r\n\r\nNot Found";
    send(fd, http400, strlen(http400), 0);
    close(fd);
}