#include "network.h"

char *getIP(char *name)
{
    struct hostent *host_info;
    struct in_addr **addr_list;
    char ip[INET_ADDRSTRLEN] = "";

    host_info = gethostbyname(name);

    if (host_info == NULL)
    {
        herror("gethostbyname");
        return NULL;
    }

    addr_list = (struct in_addr **)host_info->h_addr_list;

    if (addr_list[0] == NULL)
    {
        fprintf(stderr, "No IP address found for %s\n", name);
        return NULL;
    }

    // Convert the binary IP address to a human-readable format
    inet_ntop(AF_INET, addr_list[0], ip, INET_ADDRSTRLEN);

    char *ip_final = (char *)malloc(INET_ADDRSTRLEN);
    if (ip_final == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(ip_final, ip);
    return ip_final;
}

int TCP_connect(char *ip, char *url2)
{
    if (ip != NULL)
    {
        // printf("IP address: %s\n", ip);

        int sockfd;
        struct sockaddr_in servaddr;

        // socket create and verification
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
        {
            perror("socket creation failed...");
            free(ip);
            exit(EXIT_FAILURE);
        }
        else
            // printf("Socket successfully created..\n");

            bzero(&servaddr, sizeof(servaddr));

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(ip);
        servaddr.sin_port = htons(PORT);

        // connect the client socket to the server socket
        if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
        {
            perror("connection with the server failed...");
            free(ip);
            close(sockfd);
            return 0;
        }
        else{}
            //printf("connected to the server..\n");

        char request[2 * strlen(ip)];
        char *url = "man1/";
        char *arg = "man.he.net";
        sprintf(request, "GET /%s%s HTTP/1.1\r\n"
                         "Host: %s\r\n"
                         "\r\n",
                url, url2, arg);

         //printf("%s\n", request);

        if (send(sockfd, request, strlen(request), 0) == -1)
        {
            perror("send");
            free(ip);
            close(sockfd);
            return 0;
        }

        // Read and print the response body
        char buffer[100000];
        int n;

        while ((n = recv(sockfd, buffer, 100000 - 1, 0)) > 0)
        {
            buffer[n] = '\0';
        }

        if (n < 0)
        {
            perror("recv");
        }
        else
        {
            int start = 0;
            for (int i = 0; i < 99996; i++)
            {
                if (buffer[i] == 'N' && buffer[i + 1] == 'A' && buffer[i + 2] == 'M' && buffer[i + 3] == 'E' && buffer[i + 4] != '=')
                {
                    start = i;
                    break;
                }
            }
            int end = 0;
            for (int i = start; i < 100000; i++)
            {
                if (buffer[i] == 'A' && buffer[i + 1] == 'U' && buffer[i + 2] == 'T' && buffer[i + 3] == 'H' && buffer[i + 4] == 'O' && buffer[i + 5] == 'R')
                {
                    end = i;
                    break;
                }
            }
            if (start == 0)
            {
                printf("ERROR\n         No Such Command\n");
                return 0;
            }
            for (int i = start; i < end; i++)
            {
                printf("%c", buffer[i]);
            }
            printf("\n");
        }
        close(sockfd);
        // parseManPage(htmlContent); // Parse and print the man page content
        return 1;
    }
}
void imanwow(char *url)
{
    char *ip = getIP("man.he.net");

    if (ip != NULL)
    {
        TCP_connect(ip, url);
    }
    else
    {
        printf("Unable to resolve the IP address.\n");
    }
}