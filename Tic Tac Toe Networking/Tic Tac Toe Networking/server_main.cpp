// server_main.cpp
#include "TicTacToe.h"
#include <iostream>
#include <string>
#include <WinSock2.h>

int server_main(int argc, char *argv[], std::string playerName)
{
	SOCKET s;
	char buf[MAX_RECV_BUF];
	char host[MAX_HOST_SIZE];
	char port[MAX_SERVICE_SIZE];
	char response_str[MAX_SEND_BUF];
	
	s = passivesock(TicTacToe_UDPPORT,"udp");

	std::cout << std::endl << "Waiting for a challenge..." << std::endl;
	int len = UDP_recv(s, buf, MAX_RECV_BUF, host, port);	// Blocking function call!

	bool finished = false;
	while (!finished) {
		if ( strcmp(buf,TicTacToe_QUERY) == 0) {
			// Respond to name query
			strcpy_s(response_str,TicTacToe_NAME);
			strcat_s(response_str,playerName.c_str());
			UDP_send(s, response_str, strlen(response_str)+1, host, port);

		} else if ( strncmp(buf,TicTacToe_CHALLENGE,strlen(TicTacToe_CHALLENGE)) == 0) {
			// Received a challenge  
			char *startOfName = strstr(buf,TicTacToe_CHALLENGE);
			if (startOfName != NULL) {
				std::cout << std::endl << "You have been challenged by " << startOfName+strlen(TicTacToe_CHALLENGE) << std::endl;
			}
			
			// Play the game.  You are the 'O' player
			int winner = playTicTacToe(s, (char*) playerName.c_str(), host, port, O_PLAYER);
			finished = true;
		}

		if (!finished) {
			char previous_buf[MAX_RECV_BUF];		strcpy_s(previous_buf,buf);
			char previous_host[MAX_HOST_SIZE];		strcpy_s(previous_host,host);
			char previous_port[MAX_SERVICE_SIZE];	strcpy_s(previous_port,port);

			// Check for duplicate datagrams (can happen if broadcast enters from multiple ethernet connections)
			bool newDatagram = false;
			int status = wait(s,1,0);	// We'll wait a second to see if we receive another datagram
			while (!newDatagram && status > 0) {
				len = UDP_recv(s, buf, MAX_RECV_BUF, host, port);
				if (strcmp(buf,previous_buf)==0 &&		// If this datagram is identical to previous one, ignore it.
					strcmp(host,previous_host)==0 && 
					strcmp(port,previous_port)==0) {
						status = wait(s,1,0);			// Wait another second (still more copies?)
				} else {
					newDatagram = true;		// if not identical to previous one, keep it!
				}
			}

			// If we waited one (or more seconds) and received no new datagrams, wait for one now.
			if (!newDatagram ) {
				len = UDP_recv(s, buf, MAX_RECV_BUF, host, port);
			}
		}
	}
	closesocket(s);

	return 0;
}