QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += console

SOURCES += \
    IP.cpp \
    MainWindow.cpp \
    checkedport.cpp \
    main.cpp

HEADERS  += \
    IP.h \
    MainWindow.h \
    checkedport.h

FORMS    += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
