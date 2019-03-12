#ifndef THREADCAIXA_H
#define THREADCAIXA_H

#include <QThread>
#include <QDebug>
#include <cstdlib>
#include "controladorderecursos.h"
#include "transacao.h"
#include <QQueue>
#include "transacao.h"
#include <qtcpsocket.h>
#include <QDataStream>
#include <qhostaddress.h>

class ThreadCaixa : public QThread
{
    Q_OBJECT
public:

    explicit ThreadCaixa(QObject *parent = 0);


    ThreadCaixa(ControladorDeRecursos* controladorDeRecursos,QQueue<Transacao> * fila, QQueue<Transacao> * filaPrioritaria);


    QQueue<Transacao> * fila;
    QQueue<Transacao> * filaPrioritaria;
//    bool efetuaTransacao(Transacao *transacao);


    QString comunicaAgenciaX(int x,QString mensagem);


    /** Metodo principal que dispara a thread
      */
    void run();
    /** Agencias que serão utilizadas pela ThreadCaixa */
    int agenciaAlfa,agenciaBeta;
    int chequeEspecial;

    bool realizarTransacao(Transacao *);



    /** Ponteiro para um controlador de recursos. */
    ControladorDeRecursos* controladorDeRecursos;



signals:

public slots:

};

#endif // THREADCAIXA_H
