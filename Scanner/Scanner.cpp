#include "Scanner.h"
//#include <WinSock2.h>
//#include <ws2tcpip.h>

Scanner::Scanner()//: cntThreads(8)
{
//    QObject::connect(w, SIGNAL(threadsCountIsDone()), this, SLOT(mainThread()));
}


//int Scanner::socketThread(int threadID)
//{
//    WSADATA wsData;
//    if (WSAStartup(MAKEWORD(2, 2), &wsData))
//    {
//        std::cerr << "incorrect version of sockets on os\n";
//        return WSAGetLastError();
//    }

//	int id = (int)threadID;

//    ADDRESS_FAMILY iprotocol[2] = {AF_INET, AF_INET6};
//    SOCKET sock = socket(iprotocol[ip.getType()], PROTOCOL_TYPE, 0);
//	if (sock == INVALID_SOCKET)
//	{
//		std::cerr << "socket creation is failed\n";
//		closesocket(sock);
//		return WSAGetLastError();
//	}

//	sockaddr_in info4;
//	sockaddr_in6 info6;
//	info4.sin_family = iprotocol[ip.getType()];
//	info6.sin6_family = iprotocol[ip.getType()];
//	in_addr ip4_to_num = in_addr();
//	in6_addr ip6_to_num = in6_addr();

//	if (ip.getType() == IP::IPv4 && inet_pton(iprotocol[ip.getType()], ip.getIP().c_str(), &ip4_to_num) <= 0)
//	{
//		std::cerr << "Error in IP translation to special numeric format\n";
//		closesocket(sock);
//		return WSAGetLastError();
//	}

//	if (ip.getType() == IP::IPv6 && inet_pton(iprotocol[ip.getType()], ip.getIP().c_str(), &ip6_to_num) <= 0)
//	{
//		std::cerr << "Error in IP translation to special numeric format\n";
//		closesocket(sock);
//		return WSAGetLastError();
//	}

//    info4.sin_addr = ip4_to_num;
//	info6.sin6_addr = ip6_to_num;
//    //WaitForSingleObject(userIsDone, INFINITE);

//    std::cout << id << '\n';
//    std::unique_lock<std::mutex> lock(w->mtx);
//    (w->cv).wait(lock, [&]{ return w->userIsDone; });
//    std::cout << id << '\n';
//    for (int i = id; i < w->ports.size(); i += cntThreads)
//    {
//        unsigned int port = w->ports[i];
//		info4.sin_port = htons(port);
//        info6.sin6_port = htons(port);
//        if ((ip.getType() == IP::IPv4 && ::connect(sock, (sockaddr*)&info4, sizeof(info4)) == 0) ||
//            (ip.getType() == IP::IPv6 && ::connect(sock, (sockaddr*)&info6, sizeof(info6)) == 0))
//		{
//            std::cout << "Port " << port << " is opened\n";
//			checkedPorts[id].push_back(CheckedPort(true, PROTOCOL_TYPE, port));
//		}
//		else
//		{
//            std::cout << "Port " << port << " is not opened\n";
//			//std::cout << WSAGetLastError()<<'\n';
//			checkedPorts[id].push_back(CheckedPort(false, PROTOCOL_TYPE, port));
//		}
//    }

//	closesocket(sock);
//    WSACleanup();
//	return 0;
//}

//int Scanner::mainThread()
//{
//    /*WSADATA wsData;
//    if (WSAStartup(MAKEWORD(2, 2), &wsData))
//    {
//        std::cerr << "incorrect version of sockets on os\n";
//        return WSAGetLastError();
//    }*/

//    /*HANDLE threadsCountEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"threadsCountIsDone");
//    std::cerr<<(threadsCountEvent == NULL);
//    WaitForSingleObject(threadsCountEvent, INFINITE);*/
//    //std::unique_lock<std::mutex> lock(cw->mtx);
//    //(cw->cv).wait(lock, []{ return cw->threadsCountIsDone; });
//    //std::cerr<<w->getThreadsCount()<<'\n';
//    cntThreads = w->getThreadsCount();
//    ip = w->getIP();

//    /*userIsDone = CreateEvent(NULL, TRUE, FALSE, NULL);
//    if (userIsDone == NULL)
//    {
//        std::cerr << "Cannot create event\n";
//        return GetLastError();
//    }

//    //return a.exec();

//    /*std::cout << "Enter a valid IP address\n";
//    std::string ip_str;
//    getline(std::cin, ip_str);

//    try
//    {
//        ip = IP(ip_str);
//        std::cout << ip.getType() << '\n';
//    }
//    catch (const std::invalid_argument ex)
//    {
//        std::cerr << ex.what() << '\n';
//        return -1;
//    }*/

//    threads.resize(cntThreads);
//    for (int i = 0; i < cntThreads; i++)
//    {
//        threads[i] = std::thread(&Scanner::socketThread, this, i);
//    }

//    /*int t;
//    while (true)
//    {
//        std::cout << "Which ports you want to check?\n";
//        std::cout << "1: Only one\n";
//        std::cout << "2: My personal list\n";
//        std::cout << "3: Most popular\n";
//        std::cout << "4: ALL 65536\n";
//        std::cin >> t;
//        if (t >= 1 && t <= 4)
//        {
//            break;
//        }
//        else
//        {
//            std::cout << "Please, make correct choice\n";
//        }
//    }


//    switch (t)
//    {
//    case 1:
//        std::cout << "Enter one port value\n";
//        int val;
//        while (true)
//        {
//            std::cin >> val;
//            if (val < 0 || val >= (1 << 16))
//            {
//                std::cerr << "incorrect port value\n";
//            }
//            else
//            {
//                break;
//            }
//        }

//        ports.push_back(val);
//        break;
//    case 2:
//        std::cout << "Enter interested for you ports\n";
//        while (std::cin >> val)
//        {
//            if (val >= 0 || val < (1 << 16))
//            {
//                ports.push_back(val);
//            }
//        }

//        break;
//    case 3:
//    {
//        std::ifstream popfile("popular_ports.txt");
//        if (!popfile) {
//            std::cerr << "Unable to open popular ports file\n";
//            return 1;
//        }

//        int x;
//        while (popfile >> x)
//        {
//            ports.push_back(x);
//        }

//        popfile.close();
//    }

//    break;
//    case 4:
//        ports.resize(1 << 10);
//        for (int i = 0; i < (1 << 10); i++)
//        {
//            ports[i] = i;
//        }

//        break;
//    }

//    SetEvent(userIsDone);
//    DWORD threadStatus = WaitForMultipleObjects(cntThreads, threads.data(), TRUE, INFINITE);
//    if (threadStatus >= WAIT_OBJECT_0 && threadStatus <= WAIT_OBJECT_0 + cntThreads - 1)
//    {
//        std::cout << "All ports are checked successfully\n";
//        std::vector<CheckedPort> result(ports.size());
//        int indResult = 0;
//        for (auto& i: checkedPorts)
//        {
//            for (auto& j: i)
//            {
//                result[indResult++] = j;
//            }
//        }

//        std::sort(result.begin(), result.end());

//        for (auto& i : result)
//        {
//            std::string status[2] = { "CLOSED", "OPENED" };
//            std::string protocolType[2] = {"TCP", "UDP"};
//            std::cout << status[i.result] << ' ' << protocolType[i.type - 1] << ' ' << i.port << '\n';
//        }
//    }
//    else
//    {
//        std::cout << "Something went wrong...\n";
//    }

//    CloseHandle(userIsDone);
//    for (int i = 0; i < cntThreads; i++)
//    {
//        CloseHandle(threads[i]);
//    }*/

//    //WSACleanup();

//    for (int i = 0; i < cntThreads; i++)
//    {
//        threads[i].join();
//    }

//    std::cout << "All ports are checked successfully\n";
//    std::vector<CheckedPort> result(w->ports.size());
//    int indResult = 0;
//    for (auto& i: checkedPorts)
//    {
//        for (auto& j: i)
//        {
//            result[indResult++] = j;
//        }
//    }

//    std::sort(result.begin(), result.end());

//    for (auto& i : result)
//    {
//        std::string status[2] = { "CLOSED", "OPENED" };
//        std::string protocolType[2] = {"TCP", "UDP"};
//        std::cout << status[i.result] << ' ' << protocolType[i.type - 1] << ' ' << i.port << '\n';
//    }

//    return 0;
//}

/*void Scanner::startThreads()
{
    std::thread mainT(&Scanner::mainThread, this);
    mainT.join();
}*/


