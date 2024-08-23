QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++17


SOURCES += \
    IP.cpp \
    Scanner.cpp \
    checkedport.cpp \
    main.cpp

HEADERS  += \
    IP.h \
    Scanner.h \
    checkedport.h

FORMS    += \
    Scanner.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
