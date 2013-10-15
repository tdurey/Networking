/*------------------------------------------------------------------
 * connectsock - allocate & connect a client socket using TCP or UDP
 *------------------------------------------------------------------*/

#include <iostream>
#include <WinSock2.h>

SOCKET connectsock (char *host, char *service, char *protocol) {
	// Parameter List:
	// [IN] host    : String containing host as either a DNS name or an IP address
	// [IN] service : String containing service type as name (eg. ECHO) or port number (eg. 7)
	// [IN] protocol: String containing protocol type (either "tcp" or "udp")
	// Return value : SOCKET handle of connected socket.

	//	NOTES: 1. This function requires the iostream and winsock2.h header files as well as the ws2_32.lib library file.
	//         2. The host and service parameters are not used for UDP sockets.  Empty strings may be used if protocol = "UDP".

	struct hostent  *phe;      /* pointer to host information entry     */
	struct servent  *pse;      /* pointer to service information entry  */
	struct sockaddr_in client; /* an Intenet endpoint address           */
	int				status;	   /* status of connection attempt          */
	SOCKET			s;		   /* socket handle                         */

	/* Allocate and connect socket - type depends upon protocol */
	if (_stricmp(protocol, "tcp") == 0) {
		/* Create a TCP socket */
		s = socket(AF_INET, SOCK_STREAM, 0);
		if (s < 0 || s == INVALID_SOCKET) {	
			std::cout << "Cannot create socket" << std::endl;
			return INVALID_SOCKET;
		}

		memset(&client, 0, sizeof(client));		/* zero the sockaddr_in struct */

		/* Set the address family to AF_INET (= Internet) */
		client.sin_family = AF_INET;

		/* Map service name (or number) to port number */
		short iport;     /* 16-bit port number */		
		iport = htons( (u_short) atoi(service));	/* 1st try to convert string to integer */
		if (iport == 0) {							/* if that doesn't work, call service function */
			pse = getservbyname(service, protocol);
			if (pse) {
				iport = pse->s_port;
			}
			else {
				std::cout << "Invalid service request." << std::endl;
				return INVALID_SOCKET;
			}
		}
		client.sin_port = iport;	/* Assign integer port # to field in client struct */


		/* Map host name to remote IP address, allowing for dotted quad */
		long ihost;	   /* 32-bit IP address number */
		ihost = inet_addr(host);	/* 1st try to convert from "dotted decimal notation" */
		if (ihost == INADDR_NONE) {	/* if that doesn't work, send request to DNS */
			phe = gethostbyname(host);
			if (phe)
				memmove(&ihost, phe->h_addr, phe->h_length);
			else {
				std::cout << "Invalid host" << std::endl;
				return INVALID_SOCKET;
			}
		}
		client.sin_addr.s_addr = ihost;	/* Assign integer IP address to field in client struct */


		/* Connect the socket */
		status = connect(s, (LPSOCKADDR) &client, sizeof(SOCKADDR));
		if (status == SOCKET_ERROR) {	
			std::cout << "Remote host/service not found - or connection refused" << std::endl;
			return INVALID_SOCKET;
		}
	}

	else if (_stricmp(protocol,"udp") == 0) {
		/* Create a UDP socket */
		s = socket(AF_INET, SOCK_DGRAM,  0);
		if (s < 0 || s == INVALID_SOCKET) {	
			std::cout << "Cannot create socket" << std::endl;
			return INVALID_SOCKET;
		}
	}

	else {	
		std::cout << "Invalid Protocol" << std::endl;
		return INVALID_SOCKET;
	}
		
	return s;
}