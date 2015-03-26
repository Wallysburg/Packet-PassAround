# Packet-PassAround

Project Assignment: 

NAME
   passaround --- listen for UDP packets and forward with reception host:port appended.
   passaround-test --- client for passaround. Sends a UDP packet.
      
SYNOPSIS

   passaround [-vzd] [-D debuglevel] [-p listenport] host port
   passaround-test [-v] [-D debuglevel] [-c hopcount] host port

DESCRIPTION

   Passaround listens on a port (-p option or default port) for UDP packets formated
   as a hopcount followed by an ASCII string. The ASCII string should be of the format 
   
        origin(;host:port)*
        
   and records the packet's forwardings. The receiving host appends the host:port 
   information of from where the packet was received.
   
   The hopcount is decremented and unless the hop count is zero sends a UDP packet with
   the hopcount followed by the ASCII string to the host and port given in the command line.
   
   If not forwarded (hopcount is zero, or the -z flag is set), passaround prints out the ASCII 
   portion of the packet, after having appended the host:port information for the last hop.
   
   Passaround-test sends a single UDP packet to host:port with message "origin" and hopcount
   1 (or as given by the -c option) and exits.

   The hopcount is a 2 byte short in network endian order, and is the first 2 bytes of 
   the packet. The entire message is limited to 2048 bytes. No null character terminator 
   is sent for the ASCII string. 
   
OPTIONS

  -z ignore hop count and print received UDP packet information, without forwarding.
     Else silently forward packet to host at port.
  -d deamon mode. After each UDP packet forward, return to listening for the next packet.
     If not in this mode, exit. 
  -v verbose
  -D debug level, takes an positive integer 0 (default) through [your choice].
  -p Port number to listen to for incoming packets. Default is 3333.
  
ARGUMENTS

  host hostname to forward packet
  port port number on hostname to forward packet 

RETURNS
  
  None
  
AUTHOR

 Charles Davis
