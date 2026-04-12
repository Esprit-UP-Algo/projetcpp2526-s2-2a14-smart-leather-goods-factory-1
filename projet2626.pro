QT       += core gui sql charts printsupport network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

MOC_DIR     = build/moc
RCC_DIR     = build/rcc
UI_DIR      = build/ui
OBJECTS_DIR = build/obj

# --- Fix MinGW : qcustomplot.o too many sections / file too big ---
win32:mingw {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    QMAKE_CFLAGS   += -Wa,-mbig-obj
}

# Réduire la taille en Debug
QMAKE_CXXFLAGS_DEBUG += -O2
QMAKE_CFLAGS_DEBUG   += -O2
QMAKE_CXXFLAGS_DEBUG -= -g
QMAKE_CFLAGS_DEBUG   -= -g

QMAKE_CXXFLAGS += -fno-keep-inline-dllexport

SOURCES += \
    ajout.cpp \
    connection.cpp \
    faceid.cpp \
    fournisseurs.cpp \
    login.cpp \
    main.cpp \
    commandes.cpp \
    matieres.cpp \
    modifier.cpp \
    pagechat.cpp \
    pageemployee.cpp \
    pagemachine.cpp \
    produitswindow.cpp \
    produit.cpp \
    ajout_produit.cpp \
    modifier_produit.cpp \
    qcustomplot.cpp \
    smtp.cpp

HEADERS += \
    ajout.h \
    commandes.h \
    connection.h \
    faceid.h \
    fournisseurs.h \
    login.h \
    matieres.h \
    modifier.h \
    pagechat.h \
    pageemployee.h \
    pagemachine.h \
    produitswindow.h \
    produit.h \
    ajout_produit.h \
    modifier_produit.h \
    qcustomplot.h \
    smtp.h

FORMS += \
    commandes.ui \
    fournisseurs.ui \
    login.ui \
    matieres.ui \
    pagechat.ui \
    pageemployee.ui \
    pagemachine.ui \
    produitswindow.ui

RESOURCES += \
    logo.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
