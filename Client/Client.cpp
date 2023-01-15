#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <string.h>
#include <string>
using namespace std;

int main()
{
	struct DataPkt_Send
	{
		int Msg_Id;
		int send_Message[2];
	};

	//starts Winsock DLLs
	WSADATA wsaData;
	if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		return 0;
	}

	//initializes socket. SOCK_STREAM: TCP
	SOCKET ClientSocket;
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) {
		WSACleanup();
		return 0;
	}

	//Connect socket to specified server
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;						//Address family type itnernet
	SvrAddr.sin_port = htons(27000);					//port (host to network conversion)
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	//IP address
	if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
		closesocket(ClientSocket);
		WSACleanup();
		return 0;
	}

	int count = 1;
	while (1)
	{
		// Create Data package to store the information that user want to send to the server
		DataPkt_Send data_send_pakage;
		data_send_pakage.Msg_Id = count;

		// Create TxBuffer
		int first;
		int second;

		cout << endl;
		cout << "Enter a Fisrt Number to transmit: ";
		cin >> first;
		cout << "Enter a Second Number to transmit: ";
		cin >> second;
		cout << endl;

		data_send_pakage.send_Message[0] = first;
		data_send_pakage.send_Message[1] = second;

		send(ClientSocket, (char*)&data_send_pakage, sizeof(data_send_pakage), 0);

		//receives RxBuffer
		char RxBuffer[128] = {};
		recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);

		// print out the message
		cout << "Msg Rx: " << RxBuffer << endl;

		count++;
	}

	//closes connection and socket
	closesocket(ClientSocket);

	//frees Winsock DLL resources
	WSACleanup();

	return 1;
}
