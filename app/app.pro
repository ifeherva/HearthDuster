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

VERSION = 0.8.0

SOURCES += src/main.cpp\
        src/gui/mainwindow.cpp \
    src/db/cardsdb.cpp \
    src/db/card.cpp \
    src/collection.cpp \
    src/duststrategy.cpp \
    src/preferences/preferences.cpp \
    src/strategies/duplicates.cpp \
    src/strategies/wild.cpp \
    src/strategies/jointduplicates.cpp \
    src/strategies/jointduplicatesgolden.cpp

HEADERS  += src/gui/mainwindow.h \
    src/db/cardsdb.h \
    src/db/card.h \
    src/collection.h \
    src/duststrategy.h \
    src/preferences/preferences.h \
    src/strategies/duplicates.h \
    src/strategies/strategies.h \
    src/strategies/wild.h \
    src/strategies/jointduplicates.h \
    src/strategies/jointduplicatesgolden.h

macx: {
    HEADERS += src/utils/macutils.h
    OBJECTIVE_SOURCES +=  src/utils/macutils.mm

    LIBS += -framework AppKit
}

FORMS    += src/gui/mainwindow.ui

macx: QMAKE_INFO_PLIST = Info.plist
macx: ICON = resources/icons/iconset.icns

INCLUDEPATH += libs/HearthMirror/include

# copy cards data
macx {
    copydata.commands = $(COPY_DIR) $$PWD/resources/Cards $$OUT_PWD/HearthDuster.app/Contents/MacOS
    copylibs.commands = mkdir -p $$OUT_PWD/HearthDuster.app/Contents/Frameworks && $(COPY_DIR) $$PWD/libs/HearthMirror/libHearthMirror.dylib $$OUT_PWD/HearthDuster.app/Contents/Frameworks/
} else {
    copydata.commands = $(COPY_DIR) $$PWD/resources/Cards $$OUT_PWD
}

macx {
    first.depends = $(first) copydata copylibs
} else {
    first.depends = $(first) copydata
}

export(first.depends)
export(copydata.commands)
macx:export(copylibs.commands)

macx {
    QMAKE_EXTRA_TARGETS += first copydata copylibs
} else {
    QMAKE_EXTRA_TARGETS += first copydata copylibs
}

RESOURCES += \
    resources.qrc

macx: LIBS += -L$$PWD/libs/HearthMirror/ -lHearthMirror

INCLUDEPATH += $$PWD/libs/HearthMirror
DEPENDPATH += $$PWD/libs/HearthMirror
