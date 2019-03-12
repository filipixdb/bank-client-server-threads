#ifndef AGDCC_H
#define AGDCC_H

#include <QObject>
#include <QVector>
#include "threadcaixa.h"
#include <QMutex>
#include "controladorderecursos.h"
#include <QQueue>
#include <QPair>
#include "transacao.h"
#include "threadfilatransacoes.h"


/** Classe que representa a agencia principal chamada agencia dcc.
  */
class AgenciaDcc : public QObject
{
    Q_OBJECT
public:
    explicit AgenciaDcc(QObject *parent = 0);



    /** Vetor de threads (uma para cada caixa)
      */
    QVector<ThreadCaixa*> * vetorThreadsCaixas;
    void startThreads();

    ThreadFilaTransacoes * threadDafila;

    /** Fila para todos os pedidos de transação.
      */
    QQueue<Transacao> * fila;

    /** Fila para as transações que aguardam liberação de recursos.
      */
    QQueue<Transacao> * filaPrioritaria;

    /** Aloca se for possível os recursos para uma instância de Thread Caixa
      \param agenciaAlfa primeira agência requisitada
      \param agenciaBeta segunda agência requisitada
      \return se conseguiu alocar os recursos
    */



      /** Ponteiro para um controlador de recursos
        */
      ControladorDeRecursos * controladorDeRecursos;



signals:

public slots:

};

#endif // AGDCC_H
