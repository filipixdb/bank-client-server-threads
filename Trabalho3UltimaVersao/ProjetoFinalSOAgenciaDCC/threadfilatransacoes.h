#ifndef THREADFILATRANSACOES_H
#define THREADFILATRANSACOES_H
#include <QQueue>
#include <QDebug>
#include <QThread>
#include "controladorderecursos.h"
#include "transacao.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QString>


class ThreadFilaTransacoes : public QThread
{
    Q_OBJECT
public:
    explicit ThreadFilaTransacoes(QObject *parent = 0);

    ThreadFilaTransacoes(QQueue<Transacao> *, ControladorDeRecursos *);

    void run();
    /** Fila de transacoes.
      */
    QQueue<Transacao> *fila;
    QFile * arquivo;
    QTextStream * streamDeTexto;
    Transacao  transacaoAtual;



    /** Controlador de recursos.
      */
    ControladorDeRecursos *controladorDeRecursos;


    /** Metodo que le do arquivo uma transacao e retorna a transacao instanciada
      */
    void leLinhaTransacoes();



signals:

public slots:

};

#endif // THREADFILATRANSACOES_H
