/*
** name: passaround-test.c
**
** author:
** date:
** last modified:
**
** from template created 31 jan 2015 by bjr
**
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<assert.h>

#include "passaround.h"

#define DEFAULT_PORT 3333
#define LOCALHOST "localhost"
#define MAXMSGLEN 2048
#define USAGE_MESSAGE "usage: passaround-test [-v] [-D debuglevel] [-c hopcount]  hostname port"
#define ORIGIN_MESSAGE "origin"
#define DEFAULT_HOPCOUNT 1
#define MAX_HOPCOUNT 20 

int g_debug = 0 ;
int g_verbose = 0 ;

int main( int argc, char * argv[] ) {

	int sockfd ;
	int ch ;
	struct sockaddr_in their_addr ;
	int numbytes, msgsize ; 
	int to_port = DEFAULT_PORT ;
	char * to_host = LOCALHOST ;
	int hopcount = DEFAULT_HOPCOUNT ;
	char buf[MAXMSGLEN] ;
	
	/* code correctness depend on shorts being 2 bytes */
	assert(sizeof(short)==2) ;
	
	while ((ch = getopt(argc, argv, "D:c:v")) != -1) {
		switch(ch) {
		case 'D':
			g_debug = atoi(optarg) ;
			break ;
		case 'v':
			g_verbose = 1 ;
			break ;
		case 'c':
			hopcount = atoi(optarg) ;
			if (hopcount<0) hopcount = DEFAULT_HOPCOUNT ;
			if (hopcount>MAX_HOPCOUNT) hopcount = MAX_HOPCOUNT ;
			break ;
		case '?':
		default:
			fprintf(stderr,"%s\n", USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
    argv += optind;
    
    if ( argc!=2 ) {
		fprintf(stderr,"%s\n", USAGE_MESSAGE) ;
		exit(0) ;
	}
	
	to_host = argv[0] ;
	to_port = atoi(argv[1]) ;

	sockfd = create_client_datagram_socket(to_port, to_host, &their_addr ) ;
	if (sockfd==-1) {
		exit(1) ;
	}
	
	/* this is tricky to get right */
	*((short *) buf) = htons(hopcount) ;
	strcpy(buf + sizeof(short),ORIGIN_MESSAGE) ;
	msgsize = strlen(ORIGIN_MESSAGE)+sizeof(short) ; 
	
	if (g_verbose) {
		int i ;
		printf("message: ") ;
		for (i=0; i<strlen(ORIGIN_MESSAGE)+2; i++) printf("%x ",buf[i]) ;
		printf("\n") ;	
		printf("tohost: %s\ntoport: %d\n", to_host, to_port) ; 
	} 

	if ((numbytes=sendto(sockfd, buf, msgsize, 0,
			(struct sockaddr *)&their_addr, sizeof(struct sockaddr)) ) == -1 ) {
		perror("sendto") ;
		exit(1) ;
	}

	close(sockfd) ;
	return 0 ;
}

