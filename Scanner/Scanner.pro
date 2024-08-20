QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += console

SOURCES += \
    Scanner.cpp \
    IP.cpp \
    MainWindow.cpp \
    checkedport.cpp \
    main.cpp

HEADERS  += \
    IP.h \
    MainWindow.h \
    Scanner.h \
    checkedport.h

FORMS    += \
    MainWindow.ui

LIBS += -lKernel32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
