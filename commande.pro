QT+=sql
QT += printsupport
QT += charts
QT+=serialport
QT       += core gui

QT += network







greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduino.cpp \
    chat.cpp \
    chat_interface.cpp \
    client.cpp \
    commande.cpp \
    connection.cpp \
    dialog.cpp \
    donutbreakdownchart.cpp \
    exportexcelobject.cpp \
    gas.cpp \
    gestion_produit.cpp \
    livreur.cpp \
    mailer.cpp \
    mailing.cpp \
    main.cpp \
    mainslice.cpp \
    mainwindow.cpp \
    monitordialog.cpp \
    produit.cpp \
    reclamation.cpp \
    sms.cpp \
    stat_combo.cpp

HEADERS += \
    arduino.h \
    chat.h \
    chat_interface.h \
    client.h \
    commande.h \
    connection.h \
    dialog.h \
    donutbreakdownchart.h \
    exportexcelobject.h \
    gas.h \
    gestion_produit.h \
    livreur.h \
    mailer.h \
    mailing.h \
    mainslice.h \
    mainwindow.h \
    monitordialog.h \
    produit.h \
    reclamation.h \
    sms.h \
    stat_combo.h

FORMS += \
    chat_interface.ui \
    dialog.ui \
    gestion_produit.ui \
    gestion_produitt.ui \
    mainwindow.ui \
    monitordialog.ui \
    stat_combo.ui \
    tempdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource_commande.qrc \
    resource_commande.qrc

DISTFILES +=
