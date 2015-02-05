#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <math.h>
#include <time.h>
 
#define MAXRCVLEN 500
#define PORTNUM 8020
 
int main(int argc, char *argv[])
{
   char buffer[MAXRCVLEN + 1]; /* +1 so we can add null terminator */
   int len, mysocket;
   struct sockaddr_in dest; 
   struct timespec end, start;
 
   mysocket = socket(AF_INET, SOCK_STREAM, 0);
 
   memset(&dest, 0, sizeof(dest));                /* zero the struct */
   dest.sin_family = AF_INET;
   dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/ 
   dest.sin_port = htons(PORTNUM);                /* set destination port number */
 
   connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
 
   len = recv(mysocket, buffer, MAXRCVLEN, 0);

   clock_gettime(CLOCK_MONOTONIC,&end);
 
   /* We have to null terminate the received data ourselves */
   buffer[len] = '\0';
 
   memcpy(&start,buffer,sizeof(start));

   printf("Received %s (%d bytes).\n", buffer, len);
 
   unsigned long long int diff = ((end.tv_sec - start.tv_sec)*pow(10,9)) + end.tv_nsec - start.tv_nsec;

   printf("Time diff = %lu\n",diff);

   close(mysocket);
   return EXIT_SUCCESS;
}
