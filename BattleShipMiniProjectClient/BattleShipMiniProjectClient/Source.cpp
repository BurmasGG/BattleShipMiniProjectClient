#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
	string ipAdress = "127.0.0.1";
	int port = 54000;

	//init winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);

	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Cant start winsock, err #" << wsResult << endl;
		return;
	}

	// create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "cant create socket, err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// fill hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);

	// connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Cant connect to server, err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// dowhile lopp to send and recieve
	char buf[4096];
	string userInput;

	do
	{
		// ask for input
		cout << ">";
		getline(cin, userInput);

		if (userInput.size() > 0)
		{
			// send text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0); // plus 1 for null termination char
			if (sendResult != SOCKET_ERROR)
			{
				// wait for response
				memset(buf, 0, 4096);
				int bytesRecieved = recv(sock, buf, 4096, 0);
				if (bytesRecieved > 0)
				{
					// echo
					cout << "SEVER>" << string(buf, 0, bytesRecieved) << endl;
				}
			}
		}
	} while (userInput.size() > 0);

	// close things
	closesocket(sock);
	WSACleanup();
}