#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>

using namespace std;
int main()
{
	struct Data_Package_rev
	{
		int msg_Id;
		int rev_mess[2];
	};

	//starts Winsock DLLs		
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	//create server socket
	SOCKET ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET) {
		WSACleanup();
		return 0;
	}

	//binds socket to address
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(27000);
	if (bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
	{
		closesocket(ServerSocket);
		WSACleanup();
		return 0;
	}
mark:
	//listen on a socket
	if (listen(ServerSocket, 1) == SOCKET_ERROR) {
		closesocket(ServerSocket);
		WSACleanup();
		return 0;
	}


	cout << "Waiting for client connection\n" << endl;

	//accepts a connection from a client
	SOCKET ConnectionSocket;
	ConnectionSocket = SOCKET_ERROR;
	if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR) {
		closesocket(ServerSocket);
		WSACleanup();
		return 0;
	}

	cout << "Connection Established" << endl;

	while (1)
	{
		// Create new data package to store the message that be received by the client
		Data_Package_rev Data_received;

		//receives RxBuffer
		char RxBuffer[128] = {};
		recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
		memcpy(&Data_received, RxBuffer, sizeof(Data_Package_rev));
		cout << "The message ID is: " << Data_received.msg_Id << endl;
		cout << "The message is: " << Data_received.rev_mess[0] << ", " << Data_received.rev_mess[1] << endl;

		// Convert to string to compare with terminate condition
		string test(RxBuffer);

		// Send the confirmation to Client
		char TxBuffer[128] = "Received";
		send(ConnectionSocket, TxBuffer, sizeof(TxBuffer), 0);

	}

	closesocket(ConnectionSocket);	//closes incoming socket
	closesocket(ServerSocket);	    //closes server socket	
	WSACleanup();					//frees Winsock resources

	return 1;
}