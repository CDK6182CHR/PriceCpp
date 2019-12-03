#-------------------------------------------------
#
# Project created by QtCreator 2019-08-07T08:50:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PriceCpp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    baseprice.cpp \
    pricelist.cpp \
    price.cpp \
    directwidget.cpp \
    triradios.cpp \
    util.cpp \
    throughprice.cpp \
    throughwidget.cpp \
    triradiosv.cpp \
    visaprice.cpp \
    visawidget.cpp \
    pricelistwidget.cpp \
    graph.cpp \
    crnet.cpp \
    milewidget.cpp \
    dialogwrapper.cpp

HEADERS += \
        mainwindow.h \
    baseprice.h \
    pricelist.h \
    price.h \
    directwidget.h \
    triradios.h \
    util.h \
    throughprice.h \
    throughwidget.h \
    triradiosv.h \
    visaprice.h \
    visawidget.h \
    pricelistwidget.h \
    graph.h \
    crnet.h \
    milewidget.h \
    dialogwrapper.h

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    list.qrc

DISTFILES += \
    Android_source/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/Android_source

