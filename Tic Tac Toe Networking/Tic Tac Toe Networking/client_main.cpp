// client_main.cpp
#include "TicTacToe.h"
#include <iostream>
#include <string>
#include <WinSock2.h>

int client_main(int argc, char *argv[], std::string playerName)
{
	char host[MAX_HOST_SIZE];
	char port[MAX_SERVICE_SIZE];
	ServerStruct server[MAX_SERVERS];

	SOCKET s = connectsock("","","udp");		// Create a socket  (Don't need to designate a host or port for UDP

	// Find all TicTacToe servers
	std::cout << std::endl << "Looking for TicTacToe servers ... " << std::endl;
	int numServers;
	getServers(s,broadcastAddress,TicTacToe_UDPPORT, server, numServers);

	if (numServers == 0) {
		std::cout << std::endl << "Sorry.  No TicTacToe servers were found.  Try again later." << std::endl << std::endl;
	} else {
		// Display the list of servers found
		std::cout << std::endl << "Found TicTacToe server";
		if (numServers == 1) {
			std::cout << ":" << "  " << server[0].name << std::endl;
		} else {
			std::cout << "s:" << std::endl;
			for (int i=0; i<numServers; i++) {
				std::cout << "  " << i+1 << " - " << server[i].name << std::endl;
			}
			std::cout << std::endl << "  " << numServers+1 << " - QUIT" << std::endl;
		}
		std::cout << std::endl;

		// Allow user to select someone to challenge
		int answer = 0;
		std::string answer_str;
		if (numServers == 1) {
			std::cout << "Do you want to challenge " << server[0].name << "? ";
			std::getline(std::cin, answer_str);
			if (answer_str[0] == 'y' || answer_str[0] == 'Y') answer = 1;
		} else if (numServers > 1) {
			std::cout << "Who would you like to challenge (1-" << numServers+1 << ")? ";
			std::getline(std::cin,answer_str);
			answer = atoi(answer_str.c_str());
			if (answer > numServers) answer = 0;
		}
			
		if (answer >= 1 && answer <= numServers) {
			// Extract the opponent's info from the server[] array
			char serverName[MAX_SERVER_NAME];
			strcpy_s(serverName, server[answer-1].name);		// Adjust for 0-based array
			strcpy_s(host, server[answer-1].host);
			strcpy_s(port, server[answer-1].port);

			// Append playerName to the TicTacToe_CHALLENGE string & send a challenge to host:port
			char buf[MAX_SEND_BUF];
			strcpy_s(buf,TicTacToe_CHALLENGE);
			strcat_s(buf,playerName.c_str());
			int len = UDP_send(s, buf, strlen(buf)+1,host,port);

			// Play the game.  You are the 'X' player
			int winner = playTicTacToe(s, serverName, host, port, X_PLAYER);
		}
	}

	closesocket(s);
	return 0;
}