#-------------------------------------------------
#
# Project created by QtCreator 2017-04-23T19:35:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OS2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    process.cpp \
    memory.cpp \
    mem.cpp \
    Segment.cpp \
    memory_share.cpp \
    File.cpp \
    filemanager.cpp \
    openedfile.cpp \
    DiskManager.cpp \
    tree.cpp \
    file_control.cpp \
    pro_dialog.cpp \
    file_dialog.cpp \
    mem_dialog.cpp \
    text_dialog.cpp

HEADERS  += mainwindow.h \
    process.h \
    memory_share.h \
    Segment.h \
    mem.h \
    File.h \
    filemanager.h \
    head.h \
    openedfile.h \
    DiskManager.h \
    tree.h \
    file_control.h \
    pro_dialog.h \
    file_dialog.h \
    mem_dialog.h \
    text_dialog.h

FORMS    += mainwindow.ui \
    pro_dialog.ui \
    file_dialog.ui \
    mem_dialog.ui \
    text_dialog.ui

RESOURCES += \
    background.qrc
