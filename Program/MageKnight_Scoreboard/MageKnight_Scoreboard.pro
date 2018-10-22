#-------------------------------------------------
#
# Project created by QtCreator 2017-04-01T09:28:49
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MageKnight_Scoreboard
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


SOURCES +=\
    Action.cpp \
    GameTimer.cpp \
    Player.cpp \
    Main.cpp \
    Monster.cpp \
    Unit.cpp \
    City.cpp \
    qcustomplot.cpp \
    Dialog_AddCity.cpp \
    Dialog_EndGame.cpp \
    Dialog_UserAction.cpp \
    Window_MainBoard.cpp \
    GameEngine.cpp \
    Dialog_StartMenu.cpp

HEADERS  += \
    Action.h \
    GameTimer.h \
    Player.h \
    Monster.h \
    Unit.h \
    City.h \
    qcustomplot.h \
    Dialog_AddCity.h \
    Dialog_EndGame.h \
    Dialog_UserAction.h \
    Window_MainBoard.h \
    GameEngine.h \
    CharacterDescriptionTexts.h \
    Dialog_StartMenu.h

FORMS    += \
    Dialog_AddCity.ui \
    Dialog_EndGame.ui \
    Dialog_UserAction.ui \
    Window_MainBoard.ui \
    Dialog_StartMenu.ui

RESOURCES += \
    resources.qrc
