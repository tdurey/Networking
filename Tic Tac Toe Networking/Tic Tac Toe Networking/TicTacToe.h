// TicTacToe.h
//
// This header file establishes some constants that should be used in the TicTacToe project for COMP 311
#include <WinSock2.h>
#include <string>

// These 2 settings may have to be changed:
static char broadcastAddress[] = "10.1.255.255";	// Must be set to reflect the broadcast address of current LAN
static char TicTacToe_UDPPORT[] = "28900";			// Port number used by TicTacToe servers

#define TicTacToe_QUERY  "Who is hosting Tic-Tac-Toe?"
#define TicTacToe_NAME	 "Name="
#define TicTacToe_CHALLENGE "Player="

const int MAX_SEND_BUF  = 2048;
const int MAX_RECV_BUF  = 2048;
const int MAX_SERVERS   = 100;
const int MAX_SERVER_NAME = 80;
const int MAX_HOST_SIZE = 16;
const int MAX_SERVICE_SIZE = 80;

const int X_PLAYER = 1;
const int O_PLAYER = 2;

const int ABORT    = -1;
const int noWinner = 0;
const int xWinner  = 1;
const int oWinner  = 2;
const int TIE      = 3;

static char board[10];

struct ServerStruct {
	char name[MAX_SERVER_NAME];
	char host[MAX_HOST_SIZE];
	char port[MAX_SERVICE_SIZE];
};

// Utility functions for COMP 311 projects
SOCKET connectsock (char*, char*, char*);
SOCKET passivesock (char*, char*);
int UDP_recv (SOCKET, char*, int, char*, char*);
int UDP_send (SOCKET, char*, int, char*, char*);
int wait(SOCKET, int, int);

// Functions specifically used by this project
int server_main(int, char*[],std::string);
int client_main(int, char*[],std::string);
void getServers(SOCKET, char*, char*, ServerStruct[], int&);
int playTicTacToe(SOCKET, char*, char*, char*, int);

