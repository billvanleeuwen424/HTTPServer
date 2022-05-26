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

    int clientSocket;

    while(1){
        
        struct sockaddr_storage client;
        socklen_t clientlen = sizeof(client);

        if((clientSocket = accept(listen_socket, (struct sockaddr*) &client, &clientlen)) < 0){
            fprintf(stderr, "Error accepting connection. %s\n", strerror(errno));
            close(clientSocket);
            close(listen_socket);
            return -1;
        }

        //log connection to console
        char addrstr[100];
        getnameinfo((struct sockaddr*)&client, clientlen, addrstr, sizeof(addrstr), 0,0,NI_NUMERICHOST);
        printf("Connection from: %s\n", addrstr);

        //fork process to deal with client
        int pid = fork();
        if(pid == 0){   //if child process

            //loop until exit()
            while(1){
                
                close(listen_socket);

                //recieve HTTP request from client
                char httpRequest[2048];
                int bytes_recieved = recv(clientSocket, httpRequest, sizeof(httpRequest), 0);
                
                //exit if client disconnects or error.
                if(bytes_recieved < 1){
                    close(clientSocket);
                    exit(1);
                }


                //printf("%s\n", httpRequest);    //debug


                //get the first line, split on the newline
                //the first line usually contains GET / HTTP/1.1
                char *request = strtok(httpRequest, "\n");

                //check if the request is valid http, or send 404
                if(strncmp("GET /", httpRequest, 5) == 0){
                    
                    //get the request, except the "GET /"" at the front
                    char *unParsedPath = httpRequest + 4;

                    //get the filepath of the request
                    char *path = strtok(unParsedPath, " ");
                    
                    printf("%s", path); //debug
                    



                    //directory traversal check
                    if(strstr(path, "..")){
                        http400(clientSocket);
                        exit(1);
                    }

                    //check if path too long
                    if(strlen(path) > 100){
                        http400(clientSocket);
                        exit(1);
                    }   
                    
                    //serve standard file
                    if (strcmp(path, "/") == 0){
                        path = "/index.html";
                    }

                    //open file, check if exists
                    FILE *fp = fopen(path, "rb");
                    if(!fp){
                        http404(clientSocket);
                        exit(1);
                    } 

                    //seek to end, check for errors
                    if(!fseek(fp, 0L, SEEK_END)){
                        http500(clientSocket);
                        exit(1);
                    }

                    //get file size and type
                    size_t fileLength = ftell(fp);
                    char *contentType = getContentType(path);

                    //send a 200, and the file
                    http200(clientSocket, fileLength, contentType);
                    sendContent(clientSocket, fp);
                    exit(0);
                    
                }
                else{
                    http404(clientSocket);
                    exit(1);
                }
                

            }   //while 1 inside the child process
        }   //if pid == 0
    }   //while 1 outside the fork

    close(listen_socket);
    return 0;
}



//GET / HTTP/1.1
