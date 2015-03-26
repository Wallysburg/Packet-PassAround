/*
** name: passaround.h
**
** author:
** date:
** last modified:
**
** from template created 31 jan 2015 by bjr
**
*/

/* Given a port, and a sockaddr structure, returns a newly created server socket 
 * listening on that port. Returns -1 of there is a problem
 */
int create_server_datagram_socket(short myport, struct sockaddr_in * my_addr ) ;


/* Given a port, a hostname and a sockaddr structure, returns a newly created client 
 * socket that will send to that host:port. Fills in their_addr for use in send.
 * Returns -1 if there is a problem.
 */
int create_client_datagram_socket(short myport, char * hostname, struct sockaddr_in * their_addr ) ;
