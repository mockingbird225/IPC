#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <math.h>
#include<time.h>
#include <sys/time.h>
 
#define MAXRCVLEN 500
//#define PORTNUM 8020

struct message {
  struct timespec sent_time;
  char* msg;
};
 
int main(int argc, char *argv[])
{
	struct message m;
	int i, j;
	int portNum = atoi(argv[1]);
	char buffer[MAXRCVLEN + 1]; /* +1 so we can add null terminator */
	int mysocket;
	unsigned long long int minTime = 100, len;
	struct sockaddr_in dest; 
	//struct timeval end;
	struct timeval end;
	mysocket = socket(AF_INET, SOCK_STREAM, 0);

	memset(&dest, 0, sizeof(dest));                /* zero the struct */
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/ 
	dest.sin_port = htons(portNum);                /* set destination port number */

	connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));

	for (i = 0; i < 10; i++) {
		len = recv(mysocket, &m, MAXRCVLEN, 0);
		//gettimeofday(&end, NULL); 
		clock_gettime(CLOCK_MONOTONIC, &end);
		unsigned long long int diff = ((end.tv_sec - (m.sent_time).tv_sec)*pow(10,9)) + 
			end.tv_nsec - (m.sent_time).tv_nsec;
		if (diff < minTime) { minTime = diff; } 
	}
	/* We have to null terminate the received data ourselves */
	printf("Time diff in microseconds = %lu\n", minTime);
	close(mysocket);
	return EXIT_SUCCESS;
}
