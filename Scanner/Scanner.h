#ifndef SCANNER_H
#define SCANNER_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
#include <condition_variable>
#include <mutex>
#include <QComboBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QtNetwork>
#include <atomic>
#include "checkedport.h"
#include "IP.h"

#include <QLabel>


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

    unsigned short int cntThreads;
    IP ip;

    std::vector<std::thread> threads;
    std::vector<CheckedPort::protocolType> protocols;

    QLineEdit* enterIP;

    QLabel* enterThreadsCountLabel;
    QLabel* enterIPLabel;
    QLabel* invalidIPLabel;

    QLabel* portsTypeLabel;
    QLabel* portsLabel;
    QComboBox* portsType;
    QComboBox* portsBox;
    QLineEdit* enterPorts;
    QLineEdit* enterUdpMessage;
    QPushButton* startScanningBtn;
    QLabel* invalidPortsLabel;

    void getPorts();

    std::atomic<bool> isStopPushed;
    std::atomic<bool> isExitPushed;
    bool mainThreadIsActive;

    QTableWidget* tableWidget;

    std::vector<unsigned int> ports;
    std::vector<std::vector<CheckedPort> > checkedPorts;

    std::condition_variable cv;
    bool userIsDone;
    std::mutex mtx;
    int socketThread(int threadID);
    int mainThread();

    void parseEnteredPorts(const QStringList& stringPorts);
public:
    unsigned short int getThreadsCount()const;
    IP getIP()const;
public slots:
    void threadsSlot();
    void showUdpLineEdit(int id);
    void showPortsLineEdit(int id);
    void start();
protected:
    void closeEvent(QCloseEvent *event) override;

};

#endif
