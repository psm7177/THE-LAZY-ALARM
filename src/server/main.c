
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <pthread.h>
#include <protocol.h>
#include <alarm.h>
#include <list.h>
#include <response.h>
#include <alarm-context.h>
#include <mission.h>

#define MAX_CONNECTOINS 5
#define PORT 8080
#define SA struct sockaddr

pthread_mutex_t connection_mutex;
pthread_cond_t connection_condition;

int accepted_fd;

void connection_handler(int connfd, request_t *request_buff, response_t *response_buff)
{
    while (1)
    {
        bzero(request_buff, sizeof(request_t));

        int bytes_size = recv(connfd, request_buff, sizeof(request_t), 0);
        if (bytes_size == 0)
        {
            printf("%d fd is disconnected.\n", connfd);
            break;
        }
        response(request_buff, response_buff);

        bytes_size = send(connfd, response_buff, sizeof(response_t), 0);
        if (bytes_size == 0)
        {
            printf("%d fd is disconnected.\n", connfd);
            break;
        }
    }
}
void *connection_thread_handler()
{
    int connfd;
    request_t request_buff;
    response_t response_buff;

    while (1)
    {
        pthread_mutex_lock(&connection_mutex);
        while (accepted_fd == 0)
        {
            pthread_cond_wait(&connection_condition, &connection_mutex);
        }
        connfd = accepted_fd;
        accepted_fd = 0;
        pthread_mutex_unlock(&connection_mutex);
        connection_handler(connfd, &request_buff, &response_buff);
    }
}

// Driver function
int main()
{
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cli;

    pthread_t thread_pool[MAX_CONNECTOINS];
    pthread_t checking_alarm, handling;

    memset(thread_pool, 0, sizeof(thread_pool));

    pthread_mutex_init(&connection_mutex, NULL);
    pthread_cond_init(&connection_condition, NULL);

    // socket create and verification
    init_alarm_list();
    init_mission_list();
    init_alarm_sig();

    pthread_create(&checking_alarm, 0, &check_alarm, 0);
    pthread_create(&handling, 0, &signal_handler, 0);

    for (int i = 0; i < MAX_CONNECTOINS; i++)
    {
        pthread_create(thread_pool + i, NULL, connection_thread_handler, NULL);
    }
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    while (1)
    {
        connfd = accept(sockfd, (SA *)&cli, &len);
        if (connfd < 1)
        {
            printf("server accpet faild... \n");
        }
        pthread_mutex_lock(&connection_mutex);
        accepted_fd = connfd;
        pthread_mutex_unlock(&connection_mutex);
        pthread_cond_signal(&connection_condition);
    }

    connfd = accept(sockfd, (SA *)&cli, &len);
    printf("connfd: %x\n", connfd);
    if (connfd < 0)
    {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    // After chatting close the socket
    close(sockfd);
    return 0;
}