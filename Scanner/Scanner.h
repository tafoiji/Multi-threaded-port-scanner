#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
//#include <WinSock2.h>
//#include <WS2tcpip.h>
//#include <ws2tcpip.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <QApplication>
//#include "MainWindow.h"
//#include "checkedport.h"
#include <QThread>

#include "IP.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib,"user32")

//using comment::functionName;

class Scanner : public QObject {
    Q_OBJECT

public:
    //MainWindow* w;
    int socketThread(int);

    Scanner();
private:
    //DWORD cntThreads;

    std::vector<unsigned int> ports;
    //std::vector<std::vector<CheckedPort> > checkedPorts;

    IP ip;
    //HANDLE userIsDone;
    std::vector<std::thread> threads;
    //DWORD PROTOCOL_TYPE = SOCK_STREAM;


//public slots:
  //  int mainThread();
};

#endif // SCANNER_H
