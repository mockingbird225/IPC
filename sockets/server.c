#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
 
#define PORTNUM 8020

struct message {
  struct timeval sent_time;
  char* msg; 
};
 
int main(int argc, char *argv[])
{
    struct message m;
    int len=atoi(argv[1]);
    m.msg = (char *)malloc(atoi(argv[1])*sizeof(char));
    struct sockaddr_in dest; /* socket info about the machine connecting to us */
    struct sockaddr_in serv; /* socket info about our server */
    int mysocket;            /* socket used to listen for incoming connections */
    socklen_t socksize = sizeof(struct sockaddr_in);
 
    memset(&serv, 0, sizeof(serv));           /* zero the struct before filling the fields */
    serv.sin_family = AF_INET;                /* set the type of connection to TCP/IP */
    serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
    serv.sin_port = htons(PORTNUM);           /* set the server port number */    
 
    mysocket = socket(AF_INET, SOCK_STREAM, 0);
 
    /* bind serv information to mysocket */
    bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));
 
    printf("Sending...\n");
   
    /* start listening, allowing a queue of up to 1 pending connection */
    listen(mysocket, 1);
    int consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);	
    //struct timespec start;
    //clock_gettime(CLOCK_MONOTONIC,&start);

    //memcpy(msg,&start,sizeof(start));
    gettimeofday(&m.sent_time, NULL);
    send(consocket, &m, sizeof(m), 0); 
    close(consocket);
    
    close(mysocket);
    return EXIT_SUCCESS;
}

