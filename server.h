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
//sends a basic 400 response string
void http400(int fd) {
    const char *http400 = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 15\r\n\r\n400 Bad Request";
    send(fd, http400, strlen(http400), 0);
    close(fd);
}
//sends a basic 404 response string
void http404(int fd) {
    const char *http404 = "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: 13\r\n\r\n404 Not Found";
    send(fd, http404, strlen(http404), 0);
    close(fd);
}
//sends a basic 418 response string
void http418(int fd) {
    const char *http418 = "HTTP/1.1 418 I am a teapot\r\nConnection: close\r\nContent-Length: 17\r\n\r\n418 I am a teapot";
    send(fd, http418, strlen(http418), 0);
    close(fd);
}


//serves the requested resource
//returns 0 on success, -1 on directory traversal attack, 1 on resource not found
int serve(char *path){

    //directory traversal check
    if(strstr(path, "..")){
        return -1 
    }

    //check if file exists
    if(access(path, F_OK) == -1){
        return 1;   
    }

    if(path > 100)
    
    else{

    }
}
