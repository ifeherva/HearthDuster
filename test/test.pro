#-------------------------------------------------
#
# Project created by QtCreator 2017-11-12T22:04:33
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_hearthduster
CONFIG   += console
CONFIG   -= app_bundle

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


HEADERS  += ../app/src/db/cardsdb.h \
        ../app/src/db/card.h \
        ../app/src/collection.h \
        ../app/src/duststrategy.h \
        ../app/src/strategies/jointduplicates.h \
        ../app/src/strategies/jointduplicatesgolden.h

SOURCES += \
        ../app/src/db/cardsdb.cpp \
        ../app/src/db/card.cpp \
        ../app/src/duststrategy.cpp \
        ../app/src/strategies/jointduplicates.cpp \
        ../app/src/strategies/jointduplicatesgolden.cpp \
        strategies/tst_strategies.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
