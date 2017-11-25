#-------------------------------------------------
#
# Project created by QtCreator 2017-04-30T11:58:22
#
#-------------------------------------------------

QT       += core gui network

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
DEFINES += APP_VERSION=\\\"$$VERSION\\\"


CONFIG(debug, debug|release) {
    DESTDIR= $$_PRO_FILE_PWD_/../build/debug/ #Target file directory
} else {
    DESTDIR= $$_PRO_FILE_PWD_/../build/release/ #Target file directory
}

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
    src/strategies/jointduplicatesgolden.cpp \
    src/gui/aboutdialog.cpp \
    src/strategies/cardpopularityduststrategy.cpp

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
    src/strategies/jointduplicatesgolden.h \
    src/gui/aboutdialog.h \
    src/strategies/cardpopularityduststrategy.h

macx {
    HEADERS += src/utils/macutils.h
    OBJECTIVE_SOURCES +=  src/utils/macutils.mm

    LIBS += -framework AppKit
    LIBS += -L$$PWD/libs/HearthMirror/ -lHearthMirror
} else {
    HEADERS += src/utils/winutils.h
    SOURCES += src/utils/winutils.cpp

    LIBS += -L$$PWD/libs/HearthMirror/x64/ -lHearthMirror
}

FORMS    += src/gui/mainwindow.ui \
    src/gui/aboutdialog.ui

macx: QMAKE_INFO_PLIST = Info.plist
macx {
    ICON = resources/icons/iconset.icns
} else {
    RC_FILE = hearthduster.rc
}

INCLUDEPATH += libs/HearthMirror/include

macx: QMAKE_CXXFLAGS += -save-temps

# copy cards data
macx {
    copydata.commands = $(COPY_DIR) $$PWD/resources/Cards $$DESTDIR/HearthDuster.app/Contents/MacOS
    copylibs.commands = mkdir -p $$DESTDIR/HearthDuster.app/Contents/Frameworks && $(COPY_DIR) $$PWD/libs/HearthMirror/libHearthMirror.dylib $$DESTDIR/HearthDuster.app/Contents/Frameworks/
} else {
    copydata.commands = $(COPY_DIR) $$shell_path($$PWD/resources/Cards/*) $$shell_path($$OUT_PWD/release/Cards)
}

macx {
    first.depends = $(first) copydata copylibs
} else {
    first.depends = $(first) copydata
}

export(first.depends)
export(copydata.commands)
macx:export(copylibs.commands)

# macx {
#     plistupdate.commands = /usr/libexec/PlistBuddy -c \"Add :CFBundleShortVersionString string $$VERSION\" $$QMAKE_INFO_PLIST
#     QMAKE_EXTRA_TARGETS += plistupdate
#     PRE_TARGETDEPS += plistupdate
# }

macx {
    QMAKE_EXTRA_TARGETS += first copydata copylibs
} else {
    QMAKE_EXTRA_TARGETS += first copydata copylibs
}

RESOURCES += \
    resources.qrc


INCLUDEPATH += $$PWD/libs/HearthMirror
DEPENDPATH += $$PWD/libs/HearthMirror

DISTFILES += \
    hearthduster.rc
