
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

void func(int sockfd, request_t *req)
{
    response_t res;

    bzero(&res, sizeof(response_t));
    send(sockfd, req, sizeof(request_t), 0);

    int bytes_size = recv(sockfd, &res, sizeof(response_t), 0);
    if (bytes_size == 0)
    {
        fprintf(stderr, "%d fd is disconnected.\n", sockfd);
        exit(1);
    }
    char *current_body = res.body;
    uint8_t id;
    uint8_t hour;
    uint8_t minute;
    uint8_t difficulty;
    uint8_t num_music;
    uint8_t volume;
    uint8_t num_repeat;
    switch (res.type)
    {
    case TYPE_ERROR:
        fprintf(stderr, "ERROR: %s\n", res.body);
        exit(1);
    case TYPE_ALARM:
        printf("id\ttime\tdifficulty\tmusic_number\tvolume\trepeat\n");
        printf("total: %d\n", res.num_info);
        printf("============================================================\n");
        for (int i = 0; i < res.num_info; i++)
        {
            current_body += deserialize_char(current_body, &id);
            current_body += deserialize_time(current_body, &hour, &minute);
            current_body += deserialize_char(current_body, &difficulty);
            current_body += deserialize_char(current_body, &num_music);
            current_body += deserialize_char(current_body, &volume);
            current_body += deserialize_char(current_body, &num_repeat);
            printf("%d\t%d:%d\t%d\t%d\t%d\t%d\n", id, hour, minute, difficulty, num_music, volume, num_repeat);
        }
        break;
    case TYPE_MSG:
        fprintf(stdout, "%s\n", current_body);
        break;
    }
}

int main(int argc, char **argv)
{

    arg_t arg;

    parse_arg(&arg, argc, argv);

    request_t *req = make_request(&arg);

    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
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
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
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