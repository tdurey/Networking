//AUTHOR: THANE DUREY
#include "TicTacToe.h"
#include <WinSock2.h>
#include <iostream>
#include <string>

// getServers.cpp
//  Given a socket, a broadcast address and a port number, this function returns the number
//  of servers as well as an array of structs.  Each struct contains the name, IP_Address and 
//  port number of a remote server.


void getServers(SOCKET s, char *broadcastAddress, char *broadcastPort, ServerStruct server[], int &numServers)
{
//	IN parameters:
//		SOCKET s				= UDP socket handle that can be used for network communications
//		char *broadcastAddress	= The broadcast address that should be used for the current LAN
//		char *broadcastPort		= The port that should be used for the broadcast
//
//	OUT parameters:
//		ServerStruct server[]	= An array that contains a struct for each server found.  The
//								  members of each struct contain the name, IP Address and port
//								  number of a remote server.
//		numServers				= Number of elements in the server[] array
//
//	Function return value	= Number of remote servers (size of server[] array)

	numServers = 0;

	// Send TicTacToe_QUERY to broadcastAddress using broadcastPort
	
	/****
	Student should insert the necessary code here to send the TicTacToe_QUERY string
	to the broadcastAddress.  The port field should be equal to broadcastPort.
	****/

	char buff[MAX_SEND_BUF];
	strcpy(buff,TicTacToe_QUERY);
	int len = UDP_send(s, buff, MAX_SEND_BUF, broadcastAddress, TicTacToe_UDPPORT);
	// Receive incoming UDP datagrams (with a maximum of 2 second wait before each recv() function call)
	// As you read datagrams, if they start with the prefix: TicTacToe_NAME, parse out the server's name
	// and add the name, host address and port number to the server[] array.  Don't forget to increment numServers.
	// On the other hand, if the datagram doesn't start with the TicTacToe_NAME prefix, ignore it!
	
	/****
	Student should insert code here that will accomplish the task outlined in the previous
	paragraph.  After this code section executes, the server[] array should be populated with
	names, addresses & port numbers of all servers.  The integer variable, numServers, should
	contain the number of servers found.
	****/
	
	int status = wait(s, 2, 0);
	while (numServers < MAX_SERVERS && status > 0)
	{
		ServerStruct serverStruct;
		char *myHost = new char [MAX_HOST_SIZE]; 
		char *myPort = new char [MAX_SERVICE_SIZE];
		char *myBuf = new char [MAX_SERVER_NAME];
		len = UDP_recv(s, myBuf, MAX_SEND_BUF, myHost, myPort);

		if(strncmp(myBuf, TicTacToe_NAME, 5)==0)
		{
			std::string name = myBuf;
			strcpy(serverStruct.name,name.substr(5,name.length()).c_str());
			strcpy(serverStruct.host,myHost);
			strcpy(serverStruct.port,myPort);
			server[numServers] = serverStruct;
			numServers++;
		}
		status = wait(s, 2, 0);
		delete []myHost;
		delete []myPort;
		delete []myBuf;
	}	

	

}
