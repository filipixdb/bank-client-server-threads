#include "threadfilatransacoes.h"

ThreadFilaTransacoes::ThreadFilaTransacoes(QObject *parent) :
    QThread(parent)
{

}

ThreadFilaTransacoes::ThreadFilaTransacoes(QQueue<Transacao > * qQueue, ControladorDeRecursos * controladorDeRecursos2)
{
    this->fila = qQueue;
    this->controladorDeRecursos = controladorDeRecursos2;
}



void ThreadFilaTransacoes::run(){
    arquivo = new QFile("testandofila.txt");
    if(!arquivo->open(QIODevice::ReadOnly)) qDebug()<< "Erro ao abrir arquivo de transacoes.";

    streamDeTexto = new QTextStream(arquivo);
    Transacao transacaoDeTeste;
    int i=0;
    while (!streamDeTexto->atEnd()) {
        //qDebug() << "entrei no while";
        leLinhaTransacoes();
        while(transacaoAtual.tempo>i){
            sleep(1);
            i++;

        }
        //qDebug()<<transacaoAtual->agenciaOrigem<< " agencia origem";
        //qDebug()<<transacaoAtual->agenciaDestino<< " agencia destino";
        controladorDeRecursos->mutexFila.lock();
        fila->enqueue(transacaoAtual);
        controladorDeRecursos->mutexFila.unlock();
    }

}



void ThreadFilaTransacoes::leLinhaTransacoes(){
    QString linha;
    linha = streamDeTexto->readLine();

    QStringList camposDaLinha;

    camposDaLinha = linha.split(":");

    int tipo, agenciaOrigem, contaOrigem, agenciaDestino, contaDestino, valor, tempo;

    tipo = (camposDaLinha.at(0)).toInt();
    agenciaOrigem = (camposDaLinha.at(1)).toInt();
    contaOrigem = (camposDaLinha.at(2)).toInt();
    agenciaDestino =(camposDaLinha.at(3)).toInt();
    contaDestino = (camposDaLinha.at(4)).toInt();
    valor = (camposDaLinha.at(5)).toInt();
    tempo = (camposDaLinha.at(6)).toInt();
    //qDebug()<<"le linha agencia origem"<<agenciaOrigem<<" agencia destino"<<agenciaDestino;
    transacaoAtual =  Transacao(tipo, agenciaOrigem, contaOrigem, agenciaDestino, contaDestino, valor, tempo);
}
