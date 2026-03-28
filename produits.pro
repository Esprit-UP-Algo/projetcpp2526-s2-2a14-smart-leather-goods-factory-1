QT       += core gui
QT       += sql
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    connection.cpp \
    main.cpp \
    produitswindow.cpp \
    produit.cpp

HEADERS += \
    connection.h \
    produitswindow.h \
    produit.h

FORMS += \
    produitswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    logo.qrc
