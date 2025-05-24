#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX 1024
#define PORT 80
#define SA struct sockaddr

char* getIP(char* name);
int TCP_connect(char *ip,char* url2);
void imanwow(char *url);


#endif