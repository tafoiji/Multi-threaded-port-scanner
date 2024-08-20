#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //QThread mainT;

    /*DWORD mainThreadId;
    HANDLE mainT = CreateThread(NULL, 0, mainThread, NULL, NULL, &mainThreadId);
    if (mainT == NULL)
    {
        std::cerr << "Cannot create main thread\n";
        return GetLastError();
    }*/

    //std::thread mainT(sc.mainThread());
    //mainT.join();


    //WaitForSingleObject(mainT, INFINITE);
    //CloseHandle(mainT);

    return a.exec();
}
