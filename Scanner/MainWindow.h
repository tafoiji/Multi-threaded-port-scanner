#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QtWidgets/QMainWindow>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
//#include <Windows.h>
#include <condition_variable>
#include <mutex>
#include <QComboBox>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "checkedport.h"
#include "IP.h"

#include <QLabel>

#pragma comment(lib,"user32")

QT_BEGIN_NAMESPACE
namespace Ui { class Form; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::Form *ui;

    QPushButton* btnThreadsCount;
    QSpinBox* threadsCount;

    DWORD cntThreads;
    IP ip;

    std::vector<std::thread> threads;
    std::vector<DWORD> PROTOCOL_TYPE;

    QLineEdit* enterIP;

    QLabel* enterThreadsCountLabel;
    QLabel* enterIPLabel;
    QLabel* invalidIPLabel;

    QLabel* portsTypeLabel;
    QLabel* portsLabel;
    QComboBox* portsType;
    QComboBox* portsBox;
    QLineEdit* enterPorts;
    QPushButton* startScanningBtn;
    QLabel* invalidPortsLabel;

    void getPorts();

    //HANDLE threadsCountIsDone;


    bool isStopPushed;
    bool mainThreadIsActive;

    QTableWidget* tableWidget;
public:

    std::vector<unsigned int> ports;
    std::vector<std::vector<CheckedPort> > checkedPorts;
    DWORD getThreadsCount()const;
    IP getIP()const;
    std::condition_variable cv;
    bool userIsDone;
    std::mutex mtx;
    int socketThread(int threadID);
    int mainThread();
signals:
    void threadsCountIsDone();
public slots:
    void threadsSlot();
    void showPortsLineEdit(int id);
    void start();
protected:
    void closeEvent(QCloseEvent *event) override;

};

#endif
