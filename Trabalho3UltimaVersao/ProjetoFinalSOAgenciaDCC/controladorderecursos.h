#ifndef CONTROLADORDERECURSOS_H
#define CONTROLADORDERECURSOS_H

#include <QObject>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QDebug>
#include "transacao.h"
#include <QStringList>



class ControladorDeRecursos : public QObject
{
    Q_OBJECT
public:
    explicit ControladorDeRecursos(QObject *parent = 0);

    bool alocarRecursos(int agenciaAlfa,int agenciaBeta);
    bool liberarRecursos(int agenciaAlfa,int agenciaBeta);
    ~ControladorDeRecursos();


//    bool efetuaTransacao(Transacao * transacao);


    /** Mutex para alocar recursos
      */
      QMutex mutexAlocarRecursos;
      QMutex mutexFilaPrioritaria;
      QMutex mutexFila;


      /** Mutex para alocar os logs
        */
        QMutex vetorMutexLog[4];


      /** Mutex para alocar os arquivos de clientes
        */
        QMutex vetorMutexClientes[4];

        /** Ponteiro para QDataStream de log de operacoes realizadas.
           */
           QTextStream * logDeOperacao[4];


         /** Ponteiro para QDataStream de dados dos clientes.
           */
           QTextStream * streamClientes[4];


       /** Ponteiro para QFile de log de operacoes realizadas.
          */
          QFile * vetorFileLogDeOperacao[4];

       /** Ponteiro para QFile de dados dos clientes.
         */
         QFile * vetorFileClientes[4];



      /** Vetor de disponibilidade de recursos.
        *Cada recurso representa a possibilidade de pegar lock do log de clientes da agencia.
        *Inicializa com todos os recursos disponiveis.
        */

      bool vetorRecursos[4];


      /** Funcao para localizar a entrada de um cliente no banco de dados
        */
         bool localizaCliente();



signals:

public slots:

};

#endif // CONTROLADORDERECURSOS_H
