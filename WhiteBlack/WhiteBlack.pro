#-------------------------------------------------
#
# Project created by QtCreator 2017-09-21T16:22:32
#
#-------------------------------------------------

QT       += core gui

QT       += multimedia multimediawidgets

QT       += network

QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WhiteBlack
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    form.cpp \
    chessboard.cpp \
    start.cpp \
    netchess.cpp \
    sql.cpp \
    usersql.cpp \
    customslider.cpp \
    mediaplayer.cpp \
    interface.cpp

HEADERS  += widget.h \
    form.h \
    chessboard.h \
    start.h \
    netchess.h \
    sql.h \
    usersql.h \
    customslider.h \
    mediaplayer.h \
    interface.h

FORMS    += widget.ui \
    form.ui \
    chessboard.ui \
    start.ui \
    netchess.ui \
    sql.ui \
    usersql.ui \
    mediaplayer.ui \
    interface.ui

RESOURCES += \
    photo.qrc

OTHER_FILES += \
    iocn.rc


