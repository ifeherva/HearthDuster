#-------------------------------------------------
#
# Project created by QtCreator 2017-04-30T11:58:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HearthDuster
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += Src/main.cpp\
        Src/GUI/mainwindow.cpp \
    Src/Database/cardsdb.cpp \
    Src/Database/card.cpp \
    Src/collection.cpp \
    Src/duststrategy.cpp

HEADERS  += Src/GUI/mainwindow.h \
    Src/Database/cardsdb.h \
    Src/Database/card.h \
    Src/collection.h \
    Src/duststrategy.h

macx: {
    HEADERS += Src/utils/macutils.h
    OBJECTIVE_SOURCES +=  Src/utils/macutils.mm

    LIBS += -framework AppKit
}

FORMS    += Src/GUI/mainwindow.ui

INCLUDEPATH += libs/HearthMirror/include

# copy cards data
macx {
    copydata.commands = $(COPY_DIR) $$PWD/Resources/Cards $$OUT_PWD/HearthDuster.app/Contents/MacOS
} else {
    copydata.commands = $(COPY_DIR) $$PWD/Resources/Cards $$OUT_PWD
}

first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

RESOURCES += \
    resources.qrc

macx: LIBS += -L$$PWD/libs/HearthMirror/ -lHearthMirror

INCLUDEPATH += $$PWD/libs/HearthMirror
DEPENDPATH += $$PWD/libs/HearthMirror
