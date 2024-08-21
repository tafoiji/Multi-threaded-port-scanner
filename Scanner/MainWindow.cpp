#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include <memory>
#include <fstream>
#include <qDebug>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Form)//, cntThreads(8), checkedPorts(cntThreads), threads(cntThreads)
{
    ui->setupUi(this);
    userIsDone = false;
    isStopPushed = false;
    mainThreadIsActive = false;

    this->setWindowTitle("Port Scanner");
    this->setFixedWidth(300);
    this->setFixedHeight(200);

    //QObject::connect(this, SIGNAL(threadsCountIsDone()), this, SLOT(mainThread()));

    btnThreadsCount = new QPushButton("Submit");
    connect(btnThreadsCount, SIGNAL (pressed()), this, SLOT (threadsSlot()));
    threadsCount = new QSpinBox();
    threadsCount->setMinimum(1);
    threadsCount->setMaximum(10000);
    threadsCount->setValue(8);
    enterThreadsCountLabel = new QLabel("Enter count of threads");
    enterIPLabel = new QLabel("Enter IPv4 or IPv6 address");
    invalidIPLabel = new QLabel();
    enterIP = new QLineEdit;
    enterIP->setStyleSheet("QLineEdit { background-color: #353535; border: 1px solid #4F4F4F; border-radius: 5px; padding: 5px; color: white; }");
    enterIP->setPlaceholderText("IP address");

    btnThreadsCount->setStyleSheet("QPushButton { background-color: #5F9EA0; border-radius: 10px; height: 40px; } QPushButton:hover { background-color: #4682B4; }");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(enterThreadsCountLabel);
    layout->addWidget(threadsCount);
    layout->addWidget(enterIPLabel);
    layout->addWidget(enterIP);
    layout->addWidget(invalidIPLabel);
    layout->addWidget(btnThreadsCount);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    layout->setParent(widget);
    this->setCentralWidget(widget);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
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

    delete enterIP;
    delete enterThreadsCountLabel;
    delete enterIPLabel;
    delete invalidIPLabel;
    delete portsTypeLabel;
    delete portsLabel;
    delete portsType;
    delete portsBox;
    delete enterPorts;
    delete enterUdpMessage;
    delete startScanningBtn;
    delete invalidPortsLabel;

    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(0);
    delete tableWidget;
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

        checkedPorts.resize(cntThreads);
        threads.resize(cntThreads);

        std::thread mainT = std::thread(&MainWindow::mainThread, this);

        getPorts();

        mainT.detach();
    }
    catch (const std::invalid_argument& ex)
    {
        std::cerr << ex.what() << '\n';
        invalidIPLabel->setText(ex.what());
        invalidIPLabel->setStyleSheet("color: red");
    }
}

IP MainWindow::getIP()const
{
    return ip;
}

unsigned short int MainWindow::getThreadsCount()const
{
    return threadsCount->value();
}

void MainWindow::getPorts()
{
    this->setFixedWidth(450);
    this->setFixedHeight(650);

    tableWidget = new QTableWidget(this);
    portsTypeLabel = new QLabel("Choose which type of ports do you want to scan");
    portsType = new QComboBox();
    portsType->setStyleSheet("QComboBox { background-color: #353535; border: 1px solid #4F4F4F; border-radius: 5px; padding: 5px; color: white; }");
    portsType->addItem("TCP");
    portsType->addItem("UDP");
    portsType->addItem("TCP/UDP");
    enterUdpMessage = new QLineEdit();
    enterUdpMessage->setStyleSheet("QLineEdit { background-color: #353535; border: 1px solid #4F4F4F; border-radius: 5px; padding: 5px; color: white; }");
    enterUdpMessage -> setPlaceholderText("Enter message to your UDP server. «Test» by default");
    enterUdpMessage->hide();

    portsLabel = new QLabel("Choose scanning options");
    portsBox = new QComboBox();
    portsBox->setStyleSheet("QComboBox { background-color: #353535; border: 1px solid #4F4F4F; border-radius: 5px; padding: 5px; color: white; }");
    portsBox->addItem("Popular ports");
    portsBox->addItem("My list of ports");
    portsBox->addItem("All ports");

    enterPorts = new QLineEdit();
    enterPorts->setStyleSheet("QLineEdit { background-color: #353535; border: 1px solid #4F4F4F; border-radius: 5px; padding: 5px; color: white; }");
    enterPorts->setPlaceholderText("enter ports throuhg space");
    enterPorts->hide();

    invalidPortsLabel = new QLabel("invalid ports will be skipped");
    invalidPortsLabel->setStyleSheet("color: red");
    invalidPortsLabel->hide();

    startScanningBtn = new QPushButton("START SCANNING");
    startScanningBtn->setStyleSheet("QPushButton { background-color: #5F9EA0; border-radius: 10px; height: 40px; } QPushButton:hover { background-color: #4682B4; }");

    QObject::connect(startScanningBtn, SIGNAL (pressed()), this, SLOT (start()));

    QObject::connect(portsType,SIGNAL(currentIndexChanged(int)), this, SLOT(showUdpLineEdit(int)));
    QObject::connect(portsBox,SIGNAL(currentIndexChanged(int)), this, SLOT(showPortsLineEdit(int)));

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(portsTypeLabel);
    layout->addWidget(portsType);
    layout->addWidget(enterUdpMessage);
    layout->addWidget(portsLabel);
    layout->addWidget(portsBox);
    layout->addWidget(enterPorts);
    layout->addWidget(invalidPortsLabel);
    layout->addWidget(startScanningBtn);
    layout->addWidget(tableWidget);

    delete (this->centralWidget()->layout());
    (this->centralWidget())->setLayout(layout);
    layout->setParent(this->centralWidget());
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

void MainWindow::showUdpLineEdit(int id)
{
    if (id != 0)
    {
        enterUdpMessage->show();
    }
    else
    {
        enterUdpMessage->hide();
    }
}

void MainWindow::start()
{
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(0);

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
        invalidPortsLabel->setText("invalid ports will be skipped");
        QStringList stringPorts = enterPorts->text().split(" ");
        for(auto& i: stringPorts)
        {
            bool ok = false;
            int val = i.toInt(&ok);
            if (!ok && invalidPortsLabel->isHidden())
            {
                invalidPortsLabel->show();
            }
            else if (ok && val >= 0 && val < (1 << 16))
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
            isStopPushed = true;
            startScanningBtn->setText("START SCANNING");
            invalidPortsLabel->setText("Unable to open popular ports file");
            if(invalidPortsLabel->isHidden())
            {
                invalidPortsLabel->show();
            }

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
        protocols = std::vector<CheckedPort::protocolType>(1, CheckedPort::TCP);
    }
    else if (portsType->currentIndex() == 1)
    {
        protocols = std::vector<CheckedPort::protocolType>(1, CheckedPort::UDP);
    }
    else
    {
        protocols = std::vector<CheckedPort::protocolType>(2);
        protocols[0] = CheckedPort::TCP;
        protocols[1] = CheckedPort::UDP;
    }


    {
        std::lock_guard<std::mutex> lock(mtx);
        userIsDone = true;
    }
    cv.notify_all();
}

int MainWindow::socketThread(int id)
{
    QHostAddress address(ip.getIP().c_str());
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]{ return userIsDone; });
    lock.unlock();

    for (auto protocol: protocols)
    {
        for (int i = id; i < ports.size(); i += cntThreads)
        {
            if (isStopPushed)
            {
                return 0;
            }

            quint16 port = ports[i];
            if (protocol == CheckedPort::TCP)
            {
                QTcpSocket socket;

                socket.connectToHost(address, port);
                if (socket.waitForConnected(5000)) {
                    checkedPorts[id].push_back(CheckedPort(true, protocol, port));
                }
                else
                {
                    checkedPorts[id].push_back(CheckedPort(false, protocol, port));
                }
            }
            else
            {
                QUdpSocket socket;
                QHostAddress localAddr = QHostAddress::Any;
                if (!socket.bind(localAddr, 0))
                {
                    qWarning() << "Failed to bind socket:" << socket.errorString();
                    return -1;
                }

                QByteArray data = enterUdpMessage->text().toUtf8();
                if (socket.state() == QAbstractSocket::BoundState && data.size() == 0)
                {
                    socket.writeDatagram("Test", address, port);
                }
                else if (socket.state() == QAbstractSocket::BoundState)
                {
                    socket.writeDatagram(data, address, port);
                }
                else
                {
                    qWarning() << "Failed to bind socket:" << socket.errorString();
                }

                if (socket.waitForReadyRead(5000)) {
                    checkedPorts[id].push_back(CheckedPort(true, protocol, port));
                }
                else
                {
                    checkedPorts[id].push_back(CheckedPort(false, protocol, port));
                }

            }

        }
    }

    return 0;
}

int MainWindow::mainThread()
{
    mainThreadIsActive = true;
    for (auto& i: checkedPorts)
    {
        i.clear();
    }

    for (int i = 0; i < cntThreads; i++)
    {
        threads[i] = std::thread(&MainWindow::socketThread, this, i);
    }

    for (int i = 0; i < cntThreads; i++)
    {
        threads[i].join();
    }

    qDebug() << "ports are checked successfully\n";
    std::vector<CheckedPort> result(ports.size() * protocols.size());
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
    QStringList headers = {"Type", "Port", "Status"};
    tableWidget->setHorizontalHeaderLabels(headers);
    for (auto i = result.begin(); i != result.begin() + indResult; i++)
    {
        QString statusStr[2] = { "CLOSED", "OPENED" };
        QString protocolType[2] = {"TCP", "UDP"};
        tableWidget->setItem(i - result.begin(), 0, new QTableWidgetItem(protocolType[(*i).type]));
        tableWidget->setItem(i - result.begin(), 1, new QTableWidgetItem(QString::number((*i).port)));
        QTableWidgetItem* status = new QTableWidgetItem(statusStr[(*i).result]);
        if (!(*i).result)
        {
            status->setForeground(Qt::red);
        }
        else
        {
            status->setForeground(Qt::green);
        }

        tableWidget->setItem(i - result.begin(), 2, status);
    }

    startScanningBtn->setText("START SCANNING");
    mainThreadIsActive = false;
    return 0;
}
