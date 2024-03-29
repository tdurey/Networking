// TicTacToe.cpp

// This program plays Tic-Tac-Toe with other computers across the network using the UDP protocol.

#include "TicTacToe.h"
#include <iostream>
#include <string>
#include <WinSock2.h>

#pragma comment (lib, "ws2_32.lib")

int main (int argc, char *argv[])
{
	WORD wVersion = 0x0202;
	WSADATA wsaData;
	int iResult = WSAStartup (wVersion, &wsaData);
	if (iResult != 0) {
		std::cout << "Unable to initialize Windows Socket library." << std::endl;
		return 0;
	}
	
	std::string playerName;
	std::cout << "What is your name? ";
	std::getline(std::cin, playerName);

	// Display a menu and get the user's response
	char choice_str[80], newline;
	int choice = 0;
	const int QUIT = 3;
	do {
		std::cout << std::endl << std::endl << std::endl;
		std::cout << "Choose an option:" << std::endl;
		std::cout << "   1 - Host a game of Tic-Tac-Toe" << std::endl;
		std::cout << "   2 - Challenge someone else" << std::endl;
		std::cout << "   3 - Quit" << std::endl;
		std::cout << "Enter 1, 2 or 3: ";
		std::cin >> choice_str; std::cin.get(newline);
		choice = atoi(choice_str);

		switch (choice) {
		case 1:	server_main(argc,argv,playerName); break;
		case 2:	client_main(argc,argv,playerName); break;
		case QUIT: break;
		default: std::cout << std::endl << "Please enter a digit between 1 and 3." << std::endl;
		}
	} while (choice != QUIT);

	WSACleanup();
}

