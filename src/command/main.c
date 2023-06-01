
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <protocol.h>
#include <parser.h>

#include <request.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd, request_t * req)
{
    char buff[MAX];
    bzero(buff, sizeof(buff));
    write(sockfd, req, sizeof(request_t));

    // read(sockfd, buff, sizeof(buff));
}
 
int main( int argc, char **argv)
{
    
    arg_t arg;

    parse_arg(&arg, argc, argv);

    request_t * req = make_request(&arg);
    
    int sockfd;
    struct sockaddr_in servaddr;
    
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
 
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
 
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
 
    // function for chat
    func(sockfd, req);
 
    // close the socket
    close(sockfd);
    return 0;
}