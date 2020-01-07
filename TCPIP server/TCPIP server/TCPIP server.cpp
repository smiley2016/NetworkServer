#include <stdio.h>
#include <iostream>
#include <sys/types.h>    
#include<ctime>
#include "SysThread.h"
#include <vector>
#include <conio.h>
#include <string>
#include "User.h"
#include "ServerThread.h"
#include <cstdlib>

#define BACKLOG 10
#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")

using namespace std;

inline void cleanup_socket(SOCKET& s) {
	closesocket(s);
	WSACleanup();
}

const int port = 165;

CRITICAL_SECTION q;

int main() {
	int x;
	char RecvBuf[1024];	int BufLen = 1024;
	srand((unsigned) time_t());

	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return 1;
	}
	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests.
	SOCKET ListenSocket;
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = ADDR_ANY;
	service.sin_port = htons(165);
	if (bind(ListenSocket,
		(SOCKADDR*)&service,
		sizeof(service)) == SOCKET_ERROR) {
		printf("bind() failed.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	//----------------------
	// Listen for incoming connection requests.
	// on the created socket

	if (listen(ListenSocket, BACKLOG) == SOCKET_ERROR) {
		printf("Error listening on socket.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	//----------------------
	// Create a SOCKET for accepting incoming requests.
	
	printf("Waiting for client to connect...\n");

	InitializeCriticalSection(&q);

	SOCKET AcceptSocket;

	while (true)
	{
		sockaddr_in addr;
		int sizeOfAddr = sizeof(addr);
		AcceptSocket = accept(ListenSocket, (SOCKADDR * ) & addr, &sizeOfAddr);
		if (AcceptSocket == INVALID_SOCKET) {
			printf("accept failed: %d\n", WSAGetLastError());
			cleanup_socket(ListenSocket);
			return 1;
		}

		printf("%s", inet_ntoa(addr.sin_addr));

		ServerThread* connection = new ServerThread(AcceptSocket);
		connection->makeThread(rand() % 50, addr, connection);
	
	}
	//-----------------------------------------------
	// Call the recvfrom function to receive datagrams
	// on the bound socket.
	

	
	//---------------------------------------------
	// When the application is finished sending, close the socket.
	printf("Exiting.\n");
	cleanup_socket(ListenSocket);
	WSACleanup();
	system("pause");
	return 0;
}
