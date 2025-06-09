#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "src/util.h"
#include "src/Socket.h"
#include "src/InetAddress.h"
#define BUFFER_SIZE 1024

void handleWriteEvent(int);
int main()
{

    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->connection(serv_addr);
    handleWriteEvent(serv_sock->getFd());
    delete serv_sock;
    delete serv_addr;
    return 0;
}
void handleWriteEvent(int sockfd)
{
    while (true)
    {
        char buf[BUFFER_SIZE];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if (write_bytes == -1)
        {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("message from server: %s\n", buf);
        }
        else if (read_bytes == 0)
        {
            printf("server socket disconnected!\n");
            break;
        }
        else if (read_bytes == -1)
        {
            close(sockfd);
            errif(true, "socket read error");
        }
    }
}
