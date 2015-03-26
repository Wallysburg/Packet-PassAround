/*
** name: passaround.c
**
** author: Charlie Davis
** date: March 2015
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
#define PORT 9876
#define USAGE_MESSAGE "usage: passaround [-vzd] [-D debuglevel] [-p listenport] host port"



int g_debug = 0 ;
int g_verbose = 0 ;

int main(int argc, char * argv[]) {
	int sockfd_l;
	int ch ;
	struct sockaddr_in my_addr;
	char * to_host = LOCALHOST ;
	int to_port = DEFAULT_PORT ; 
	int is_forcezero = 0 ;
	int is_daemon = 0 ;
	int listen_port = DEFAULT_PORT ;
	int loop = 1 ;
	//int bytes_read;
	char buf[MAXMSGLEN];
	assert(sizeof(short)==2) ; 
	
	while ((ch = getopt(argc, argv, "D:vp:dz")) != -1) {
		switch(ch) {
		case 'D':
			g_debug = atoi(optarg) ;
			break ;
		case 'v':
			g_verbose = 1 ;
			break ;
		case 'p':
			listen_port = atoi(optarg) ;
			break ;
		case 'd':
			is_daemon = 1 ;
			break ;
		case 'z':
			is_forcezero = 1 ;
			break ;
		case '?':
		default:
			printf(USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
    argv += optind;
    
    if ( argc!= 2 ) {
    	fprintf(stderr,"%s\n",USAGE_MESSAGE) ;
    	exit(0) ;
    }
    
   to_host = argv[0];
   to_port = atoi(argv[1]);

   if (g_verbose) printf("tohost: %s\ntoport: %d\n",to_host,to_port) ;
    	
	sockfd_l = create_server_datagram_socket(listen_port, &my_addr );
	if (sockfd_l==-1) {
		exit(1) ;		
	}

   while( loop ) {
     unsigned int addr_len ;
     unsigned int numbytes; // number number of bytes in buf
     struct sockaddr_in their_addr;
     char from_info[MAXMSGLEN]; 
     int bytes_sent;
     // int j, k ; 
     struct hostent * he ;

     addr_len  = sizeof(struct sockaddr_in) ;
   
     printf("listening: port %d\n", listen_port);
   
     //listen on port using recvfrom
     if ((numbytes=recvfrom(sockfd_l, buf, MAXMSGLEN-1,0,
	    (struct sockaddr *)&their_addr, &addr_len)) == -1 ) {
     	perror("recvfrom") ;
     	exit(1);
     }    

     //get source port number and hostname
     snprintf(from_info,sizeof(from_info),
		";%s:%u", inet_ntoa(their_addr.sin_addr), 
		(unsigned int) ntohs(their_addr.sin_port)) ;


        //get new port     
	if ((he=gethostbyname(to_host))==NULL) {
		perror("gethostbyname") ;
		return -1 ;
	}
	their_addr.sin_family = AF_INET ;
	their_addr.sin_port = htons((short)to_port) ;
	their_addr.sin_addr = *((struct in_addr *)he->h_addr) ;
	memset(&(their_addr.sin_zero), '\0', 8 ) ;

	buf[numbytes] = '\0' ;  // assuming there is room
	//print out stuff if g is set
	if (g_verbose) {
	  //printf("buf: |%02x|%02x|%s\n", buf[0], buf[1], buf+2) ;
		printf("hopcount: %d\n", buf[1]);
		//		printf("host: %s\n", to_host);
		//	printf("port: %d\n", to_port);
	}

	// check buf is large enough
	if ( numbytes+strlen(from_info) > sizeof(buf) ) {
		// we have a problem
		from_info[0] = '\0' ; // truncate with predjudice
	}
	//append info to end of buffer and increase numbytes by length of info	
	memcpy(buf+numbytes,from_info,strlen(from_info)) ;
	numbytes += strlen(from_info) ;
	//buf[1] is our hopcount, so if it is > 0, keep passingaround
	if(buf[1] > 0) {
	  if (g_verbose) {
	    //printf("buf: |%02x|%02x|%s\n", buf[0], buf[1], buf+2) ;                                   
	    //	    printf("hopcount: %d\n", buf[1]);
	    printf("host: %s\n", to_host);
	    printf("port: %d\n", to_port);
	  }


	  if ( buf[1] ) buf[1] -- ;
	  printf("sent: %s\n\n", buf+2); 
	  //this actually sends it
	if ((bytes_sent = sendto( sockfd_l, buf, numbytes, 0,
	       (struct sockaddr *)&their_addr, sizeof(struct sockaddr)))==-1 ) {
           perror("send") ;
           exit(1) ;
     }
      
    	//else, if hop count is 0, we just print out the appended info
	} else {
	  printf("%s",buf+2);
	}
	//flush the toilet because it's polite
	fflush(NULL);
   }
   close(sockfd_l) ;
   return 0 ;


}
