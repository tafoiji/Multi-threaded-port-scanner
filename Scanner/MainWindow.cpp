#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <qDebug>
#include <QtNetwork>
#include <QMessageBox>
#include <QCloseEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Form)//, cntThreads(8), checkedPorts(cntThreads), threads(cntThreads)
{
    ui->setupUi(this);
    userIsDone = false;
    isStopPushed = false;
    mainThreadIsActive = false;

    //QObject::connect(this, SIGNAL(threadsCountIsDone()), this, SLOT(mainThread()));

    btnThreadsCount = new QPushButton("Submit");
    connect(btnThreadsCount, SIGNAL (pressed()), this, SLOT (threadsSlot()));
    threadsCount = new QSpinBox();
    threadsCount->setMinimum(1);
    threadsCount->setMaximum(20000);
    threadsCount->setValue(8);
    enterThreadsCountLabel = new QLabel("Enter count of threads");
    enterIPLabel = new QLabel("Enter IPv4 or IPv6 address");
    invalidIPLabel = new QLabel();
    enterIP = new QLineEdit;
    enterIP->setPlaceholderText("IP address");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(enterThreadsCountLabel);
    layout->addWidget(threadsCount);
    layout->addWidget(enterIPLabel);
    layout->addWidget(enterIP);
    layout->addWidget(invalidIPLabel);
    layout->addWidget(btnThreadsCount);


    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    this->setCentralWidget(widget);

    //threadsCountIsDone = false;
    /*threadsCountIsDone = CreateEvent(NULL, TRUE, FALSE, L"threadsCountIsDone");
    if (threadsCountIsDone == NULL)
    {
        std::cerr << "Cannot create event\n";
        throw std::runtime_error("cannot create event");
        //return GetLastError();
    }*/
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //QMessageBox::information(this, "Scanner", "Please, wait");

    this->centralWidget()->setEnabled(false);

    {
        std::lock_guard<std::mutex> lock(mtx);
        userIsDone = true;
    }
    cv.notify_all();

    isStopPushed = true;
    while (mainThreadIsActive){}
    event->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::threadsSlot()
{
    try
    {
        ip = IP(enterIP->text().toStdString());
        btnThreadsCount->hide();
        threadsCount->hide();
        enterThreadsCountLabel->hide();
        enterIPLabel->hide();
        invalidIPLabel->hide();
        enterIP->hide();

        cntThreads = threadsCount->value();

        //emit threadsCountIsDone();

        checkedPorts.resize(cntThreads);
        threads.resize(cntThreads);

        std::thread mainT = std::thread(&MainWindow::mainThread, this);
        //mainThread();

        getPorts();

        mainT.detach();


        /*{
            std::lock_guard<std::mutex> lock(mtx);
            threadsCountIsDone = true;
        }
        cv.notify_one();*/

        //SetEvent(threadsCountIsDone);
        //std::cout << ip.getType() << '\n';
    }
    catch (const std::invalid_argument& ex)
    {
        std::cerr << ex.what() << '\n';
        invalidIPLabel->setText(ex.what());
        invalidIPLabel->setStyleSheet("color: red");
        //return -1;
    }


    //cntThreads = threadsCount->value();
    //checkedPorts.resize(cntThreads);
    //threads.resize(cntThreads);
}

IP MainWindow::getIP()const
{
    return ip;
}

DWORD MainWindow::getThreadsCount()const
{
    return threadsCount->value();
}

void MainWindow::getPorts()
{
    tableWidget = new QTableWidget(this);
    portsTypeLabel = new QLabel("Choose which type of ports do you want to scan");
    portsType = new QComboBox();
    portsType->addItem("TCP");
    portsType->addItem("UDP");
    portsType->addItem("TCP/UDP");

    portsLabel = new QLabel("Choose scanning options");
    portsBox = new QComboBox();
    portsBox->addItem("Popular ports");
    portsBox->addItem("My list of ports");
    portsBox->addItem("All ports");

    enterPorts = new QLineEdit();
    enterPorts->setPlaceholderText("enter ports throuhg space");
    enterPorts->hide();

    invalidPortsLabel = new QLabel("invalid ports and ports after them will be skipped");
    invalidPortsLabel->setStyleSheet("color: red");
    invalidPortsLabel->hide();

    startScanningBtn = new QPushButton("START SCANNING");
    QObject::connect(startScanningBtn, SIGNAL (pressed()), this, SLOT (start()));

    QObject::connect(portsBox,SIGNAL(currentIndexChanged(int)), this, SLOT(showPortsLineEdit(int)));

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(portsTypeLabel);
    layout->addWidget(portsType);
    layout->addWidget(portsLabel);
    layout->addWidget(portsBox);
    layout->addWidget(enterPorts);
    layout->addWidget(invalidPortsLabel);
    layout->addWidget(startScanningBtn);
    layout->addWidget(tableWidget);

    delete (this->centralWidget()->layout());
    (this->centralWidget())->setLayout(layout);
}

void MainWindow::showPortsLineEdit(int id)
{
    if (id == 1)
    {
        enterPorts->show();
    }
    else
    {
        enterPorts->hide();
    }
}


void MainWindow::start()
{
    if (startScanningBtn->text() == "START SCANNING" && mainThreadIsActive)
    {
        startScanningBtn->setText("Stop");
        isStopPushed = false;
    }
    else if (startScanningBtn->text() == "START SCANNING" && !mainThreadIsActive)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            userIsDone = false;
        }

        std::thread mainT = std::thread(&MainWindow::mainThread, this);
        mainT.detach();
        startScanningBtn->setText("Stop");
        isStopPushed = false;

    }
    else
    {
        isStopPushed = true;
        startScanningBtn->setText("START SCANNING");
    }

    if (!invalidPortsLabel->isHidden())
    {
        invalidPortsLabel->hide();
    }

    ports.clear();
    if (portsBox->currentIndex() == 1)
    {
        for (auto& c: enterPorts->text().toStdString())
        {
            if ((c != ' ') && (c < '0' || c > '9') && invalidPortsLabel->isHidden())
            {
                invalidPortsLabel->show();
            }
        }

        std::stringstream ss(enterPorts->text().toStdString());
        int val = 0;
        while (ss >> val)
        {
            if (val >= 0 || val < (1 << 16))
            {
                ports.push_back(val);
            }
            else if(invalidPortsLabel->isHidden())
            {
                invalidPortsLabel->show();
            }
        }
    }
    else if (portsBox->currentIndex() == 0)
    {
        std::ifstream popfile("popular_ports.txt");
        if (!popfile) {
            std::cerr << "Unable to open popular ports file\n";
            return;
        }

        int x;
        while (popfile >> x)
        {
            ports.push_back(x);
        }

        popfile.close();

    }
    else
    {
        ports.resize(1 << 16);
        for (int i = 0; i < (1 << 16); i++)
        {
            ports[i] = i;
        }
    }

    if (portsType->currentIndex() == 0)
    {
        PROTOCOL_TYPE = std::vector<DWORD>(1, SOCK_STREAM);
    }
    else if (portsType->currentIndex() == 1)
    {
        PROTOCOL_TYPE = std::vector<DWORD>(1, SOCK_DGRAM);
    }
    else
    {
        PROTOCOL_TYPE = std::vector<DWORD>(2);
        PROTOCOL_TYPE[0] = SOCK_STREAM;
        PROTOCOL_TYPE[1] = SOCK_DGRAM;
    }


    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(0);

    {
        std::lock_guard<std::mutex> lock(mtx);
        userIsDone = true;
    }
    cv.notify_all();
}

int MainWindow::socketThread(int id)
{
    /*WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData))
    {
        std::cerr << "incorrect version of sockets on os\n";
        return WSAGetLastError();
    }*/

    //ADDRESS_FAMILY iprotocol[2] = {AF_INET, AF_INET6};

    QHostAddress address(ip.getIP().c_str());
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]{ return userIsDone; });
    lock.unlock();

    //WaitForSingleObject(userIsDone, INFINITE);

    //QLabel* temp = new QLabel(QString((int)cntThreads));
    //temp->show();

    for (auto protocol: PROTOCOL_TYPE)
    {
        for (int i = id; i < ports.size(); i += cntThreads)
        {
            if (isStopPushed)
            {
                return 0;
            }

            quint16 port = ports[i];
            if (protocol == SOCK_STREAM)
            {
                QTcpSocket socket;

                socket.connectToHost(address, port);
                if (socket.waitForConnected(2000)) {
                    //qDebug() << "Port " << port << " is opened\n";
                    checkedPorts[id].push_back(CheckedPort(true, protocol, port));
                }
                else
                {
                    //qDebug() << "Port " << port << " is not opened\n";
                    //std::cout << WSAGetLastError()<<'\n';
                    checkedPorts[id].push_back(CheckedPort(false, protocol, port));
                }
            }
            else
            {
                QUdpSocket socket;

                socket.writeDatagram("test", address, port);
                if (socket.waitForReadyRead(2000)) {
                    //qDebug() << "Port " << port << " is opened\n";
                    checkedPorts[id].push_back(CheckedPort(true, protocol, port));
                }
                else
                {
                    //qDebug() << "Port " << port << " is not opened\n";
                    //std::cout << WSAGetLastError()<<'\n';
                    checkedPorts[id].push_back(CheckedPort(false, protocol, port));
                }

            }
            /*SOCKET sock = socket(iprotocol[ip.getType()], protocol, 0);
            if (sock == INVALID_SOCKET)
            {
                std::cerr << "socket creation is failed\n";
                closesocket(sock);
                return WSAGetLastError();
            }

            sockaddr_in info4;
            sockaddr_in6 info6;
            info4.sin_family = iprotocol[ip.getType()];
            info6.sin6_family = iprotocol[ip.getType()];
            in_addr ip4_to_num = in_addr();
            in6_addr ip6_to_num = in6_addr();

            if (ip.getType() == IP::IPv4 && inet_pton(iprotocol[ip.getType()], ip.getIP().c_str(), &ip4_to_num) <= 0)
            {
                std::cerr << "Error in IP translation to special numeric format\n";
                closesocket(sock);
                return WSAGetLastError();
            }

            if (ip.getType() == IP::IPv6 && inet_pton(iprotocol[ip.getType()], ip.getIP().c_str(), &ip6_to_num) <= 0)
            {
                std::cerr << "Error in IP translation to special numeric format\n";
                closesocket(sock);
                return WSAGetLastError();
            }

            info4.sin_addr = ip4_to_num;
            info6.sin6_addr = ip6_to_num;

            //qDebug() << ports[i] << ' ' << id << '\n';
            unsigned int port = ports[i];
            info4.sin_port = htons(port);
            info6.sin6_port = htons(port);*/


            /*if ((ip.getType() == IP::IPv4 && ::connect(sock, (sockaddr*)&info4, sizeof(info4)) == 0) ||
                (ip.getType() == IP::IPv6 && ::connect(sock, (sockaddr*)&info6, sizeof(info6)) == 0))
            {
                qDebug() << "Port " << port << " is opened\n";
                checkedPorts[id].push_back(CheckedPort(true, protocol, port));
            }
            else
            {
                qDebug() << "Port " << port << " is not opened\n";
                //std::cout << WSAGetLastError()<<'\n';
                checkedPorts[id].push_back(CheckedPort(false, protocol, port));
            }

            closesocket(sock);*/
        }
    }

    //WSACleanup();
    return 0;
}

int MainWindow::mainThread()
{
    /*WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData))
    {
        std::cerr << "incorrect version of sockets on os\n";
        return WSAGetLastError();
    }*/

    /*HANDLE threadsCountEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"threadsCountIsDone");
    std::cerr<<(threadsCountEvent == NULL);
    WaitForSingleObject(threadsCountEvent, INFINITE);*/
    //std::unique_lock<std::mutex> lock(cw->mtx);
    //(cw->cv).wait(lock, []{ return cw->threadsCountIsDone; });
    //std::cerr<<w->getThreadsCount()<<'\n';
    //cntThreads = getThreadsCount();
    //ip = getIP();

    /*userIsDone = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (userIsDone == NULL)
    {
        std::cerr << "Cannot create event\n";
        return GetLastError();
    }*/

    //return a.exec();

    /*std::cout << "Enter a valid IP address\n";
    std::string ip_str;
    getline(std::cin, ip_str);

    try
    {
        ip = IP(ip_str);
        std::cout << ip.getType() << '\n';
    }
    catch (const std::invalid_argument ex)
    {
        std::cerr << ex.what() << '\n';
        return -1;
    }*/

    mainThreadIsActive = true;
    for (auto& i: checkedPorts)
    {
        i.clear();
    }

    for (int i = 0; i < cntThreads; i++)
    {
        threads[i] = std::thread(&MainWindow::socketThread, this, i);
    }

    /*int t;
    while (true)
    {
        std::cout << "Which ports you want to check?\n";
        std::cout << "1: Only one\n";
        std::cout << "2: My personal list\n";
        std::cout << "3: Most popular\n";
        std::cout << "4: ALL 65536\n";
        std::cin >> t;
        if (t >= 1 && t <= 4)
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
        ports.resize(1 << 10);
        for (int i = 0; i < (1 << 10); i++)
        {
            ports[i] = i;
        }

        break;
    }

    SetEvent(userIsDone);
    DWORD threadStatus = WaitForMultipleObjects(cntThreads, threads.data(), TRUE, INFINITE);
    if (threadStatus >= WAIT_OBJECT_0 && threadStatus <= WAIT_OBJECT_0 + cntThreads - 1)
    {
        std::cout << "All ports are checked successfully\n";
        std::vector<CheckedPort> result(ports.size());
        int indResult = 0;
        for (auto& i: checkedPorts)
        {
            for (auto& j: i)
            {
                result[indResult++] = j;
            }
        }

        std::sort(result.begin(), result.end());

        for (auto& i : result)
        {
            std::string status[2] = { "CLOSED", "OPENED" };
            std::string protocolType[2] = {"TCP", "UDP"};
            std::cout << status[i.result] << ' ' << protocolType[i.type - 1] << ' ' << i.port << '\n';
        }
    }
    else
    {
        std::cout << "Something went wrong...\n";
    }

    CloseHandle(userIsDone);
    for (int i = 0; i < cntThreads; i++)
    {
        CloseHandle(threads[i]);
    }*/

    //WSACleanup();

    for (int i = 0; i < cntThreads; i++)
    {
        threads[i].join();
    }

    qDebug() << "All ports are checked successfully\n";
    std::vector<CheckedPort> result(ports.size() * PROTOCOL_TYPE.size());
    int indResult = 0;
    for (auto& i: checkedPorts)
    {
        for (auto& j: i)
        {
            result[indResult++] = j;
        }
    }

    std::sort(result.begin(), result.begin() + indResult);
    tableWidget->setRowCount(indResult);
    tableWidget->setColumnCount(3);
    /*QTableWidgetItem *typeHeader = new QTableWidgetItem(tr("Type"));
    typeHeader->setTextAlignment(Qt::AlignVCenter);
    QTableWidgetItem *portHeader = new QTableWidgetItem(tr("Port"));
    portHeader->setTextAlignment(Qt::AlignVCenter);
    QTableWidgetItem *statusHeader = new QTableWidgetItem(tr("Status"));
    portHeader->setTextAlignment(Qt::AlignVCenter);*/
    QStringList headers = {"Type", "Port", "Status"};
    tableWidget->setHorizontalHeaderLabels(headers);
    for (auto i = result.begin(); i != result.begin() + indResult; i++)
    {
        QString statusStr[2] = { "CLOSED", "OPENED" };
        QString protocolType[2] = {"TCP", "UDP"};
        //qDebug() << i.type << '\n';
        qDebug() << statusStr[(*i).result] << ' ' << protocolType[(*i).type - 1] << ' ' << (*i).port << '\n';
        tableWidget->setItem(i - result.begin(), 0, new QTableWidgetItem(protocolType[(*i).type - 1]));
        tableWidget->setItem(i - result.begin(), 1, new QTableWidgetItem(QString::number((*i).port)));
        QTableWidgetItem* status = new QTableWidgetItem(statusStr[(*i).result]);
        if (!(*i).result)
            status->setForeground(Qt::red);
        else
            status->setForeground(Qt::green);
        tableWidget->setItem(i - result.begin() + 1, 2, status);
    }

    startScanningBtn->setText("START SCANNING");
    mainThreadIsActive = false;
    return 0;
}

/*void Scanner::startThreads()
{
    std::thread mainT(&Scanner::mainThread, this);
    mainT.join();
}*/



