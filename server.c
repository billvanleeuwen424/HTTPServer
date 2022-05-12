#include "server.h"


int main() {

    //define address type we are looking for
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));   //zero out hints (THIS IS ABSOLOUTELY ESSENTIAL FOR SOME REASON)
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;


    //get address
    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    /*define socket*/
    int listen_socket;

    /*create socket*/
    if((listen_socket = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol)) < 0){
        fprintf(stderr, "socket create errror. %s\n", strerror(errno));
        return -1;
    }

    printf("Binding socket to local address...\n");
    /*bind*/
    if(bind(listen_socket, bind_address->ai_addr, bind_address->ai_addrlen) < 0){
        fprintf(stderr, "bind errror. %s\n", strerror(errno));
        return -1;
    }

    //release bind address mem
    freeaddrinfo(bind_address);


    /*wait for connection*/
    if(listen(listen_socket, 10) < 0){
        fprintf(stderr, "listen error. %s\n", strerror(errno));
        close(listen_socket);
        return -1;
    }

    //create a file descriptor set, add listen socket
    fd_set master;
    FD_ZERO(&master);
    FD_SET(listen_socket, &master);
    int maxSocket = listen_socket;

    printf("Waiting for connections...\n");

    int client_socket;

    while(1){
        
        struct sockaddr_storage client;
        socklen_t clientlen = sizeof(client);

        if((client_socket = accept(listen_socket, (struct sockaddr*) &client, &clientlen)) < 0){
            fprintf(stderr, "Error accepting connection. %s\n", strerror(errno));
            close(client_socket);
            close(listen_socket);
            return -1;
        }

        //log connection to console
        char addrstr[100];
        getnameinfo((struct sockaddr*)&client, clientlen, addrstr, sizeof(addrstr), 0,0,NI_NUMERICHOST);
        printf("Connection from: %s\n", addrstr);


        int pid = fork();

        if(pid == 0){

            while(1){
                
                close(listen_socket);

                //recieve request from client
                char request[1024];
                int bytes_recieved = recv(client_socket, request, 1024, 0);


                //http400(client_socket);
                //exit(1);
                http404(client_socket);
                exit(1);
            }
        }   //if pid == 0
    }   //while 1

    close(listen_socket);
    return 0;
}

