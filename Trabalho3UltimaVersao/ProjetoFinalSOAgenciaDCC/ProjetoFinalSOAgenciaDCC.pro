#-------------------------------------------------
#
# Project created by QtCreator 2011-11-20T18:44:29
#
#-------------------------------------------------

QT       += core gui
QT 	 += network

TARGET = ProjetoFinalSOAgenciaDCC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    threadcaixa.cpp \
    agenciadcc.cpp \
    controladorderecursos.cpp \
    transacao.cpp \
    threadfilatransacoes.cpp

HEADERS  += mainwindow.h \
    threadcaixa.h \
    agenciadcc.h \
    controladorderecursos.h \
    transacao.h \
    threadfilatransacoes.h

FORMS    += mainwindow.ui
