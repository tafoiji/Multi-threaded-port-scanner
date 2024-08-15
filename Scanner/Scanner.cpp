#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <string> 
#include <windows.h>
#include <stdexcept>
#include <fstream>
#include "IP.h"

#pragma comment(lib, "Ws2_32.lib")

std::vector<unsigned int> ports;

IP ip;
const DWORD cntThreads = 8;
HANDLE userIsDone;
HANDLE threads[cntThreads];


DWORD WINAPI socketThread(LPVOID threadID)
{
	int id = (int)threadID;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "socket creation is failed\n";
		closesocket(sock);
		WSACleanup();
		return WSAGetLastError();
	}

	sockaddr_in servInfo;
	servInfo.sin_family = AF_INET;
	in_addr ip_to_num;

	if (inet_pton(AF_INET, ip.getIP().c_str(), &ip_to_num) <= 0)
	{
		std::cerr << "Error in IP translation to special numeric format\n";
		closesocket(sock);
		WSACleanup();
		return WSAGetLastError();
	}

	servInfo.sin_addr = ip_to_num;
	WaitForSingleObject(userIsDone, INFINITE);
	for (int i = id; i < ports.size(); i += cntThreads)
	{
		unsigned int port = ports[i];
		servInfo.sin_port = htons(port);
		if (connect(sock, (sockaddr*)&servInfo, sizeof(servInfo)) == 0)
		{
			
			std::cout << "Port " << port << " is opened\n";
		}
		else
		{
			std::cout << "Port " << port << " is not opened\n";
		}
	}

	closesocket(sock);
	return 0;
}



int main()
{
	WSADATA wsData;
	if (WSAStartup(MAKEWORD(2, 2), &wsData))
	{
		std::cerr << "incorrect version of sockets on os\n";
		return WSAGetLastError();
	}

	userIsDone = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (userIsDone == NULL)
	{
		std::cerr << "Cannot create event\n";
		return GetLastError();
	}

	std::cout << "Enter a valid IP address\n";
	std::string ip_str;
	getline(std::cin, ip_str);

	for (int i = 0; i < cntThreads; i++)
	{
		DWORD id;
		threads[i] = CreateThread(NULL, 0, socketThread, (void*)i, CREATE_NEW_CONSOLE, &id);
		if (threads[i] == NULL)
		{
			std::cerr << "Cannot create threads\n";
			CloseHandle(userIsDone);
			for (int j = 0; j < i; j++)
			{
				CloseHandle(threads[j]);
			}

			return GetLastError();
		}
	}

	try 
	{
		ip = IP(ip_str);
	}
	catch(const std::invalid_argument ex)
	{
		std::cerr << ex.what() << '\n';
		return -1;
	}

	int t;
	while (true)
	{
		std::cout << "Which ports you want to check?\n";
		std::cout << "1: Only one\n";
		std::cout << "2: My personal list\n";
		std::cout << "3: Most popular\n";
		std::cout << "4: ALL 65536\n";
		std::cin >> t;
		if (t >= 1 || t <= 4)
		{
			break;
		}
		else
		{
			std::cout << "Please, make correct choice\n";
		}
	}


	switch (t)
	{
	case 1:
		std::cout << "Enter one port value\n";
		int val;
		while (true)
		{
			std::cin >> val;
			if (val < 0 || val >= (1 << 16))
			{
				std::cerr << "incorrect port value\n";
			}
			else
			{
				break;
			}
		}

		ports.push_back(val);
		break;
	case 2:
		std::cout << "Enter interested for you ports\n";
		while (std::cin >> val)
		{
			if (val >= 0 || val < (1 << 16))
			{
				ports.push_back(val);
			}
		}

		break;
	case 3:
		{
			std::ifstream popfile("popular_ports.txt");
			if (!popfile) {
				std::cerr << "Unable to open popular ports file\n";
				return 1;
			}

			int x;
			while (popfile >> x)
			{
				ports.push_back(x);
			}

			popfile.close();
		}

		break;
	case 4: 
		ports.resize(1 << 16);
		for (int i = 0; i < (1 << 16); i++)
		{
			ports[i] = i;
		}

		break;
	}

	SetEvent(userIsDone);
	DWORD threadStatus = WaitForMultipleObjects(cntThreads, threads, TRUE, INFINITE);
	if (threadStatus >= WAIT_OBJECT_0 && threadStatus <= WAIT_OBJECT_0 + cntThreads - 1)
	{
		std::cout << "All ports are checked successfully\n";
	}
	else
	{
		std::cout << "Something went wrong...\n";
	}

	CloseHandle(userIsDone);
	for (int i = 0; i < cntThreads; i++)
	{
		CloseHandle(threads[i]);
	}

	WSACleanup();
	system("pause");
}