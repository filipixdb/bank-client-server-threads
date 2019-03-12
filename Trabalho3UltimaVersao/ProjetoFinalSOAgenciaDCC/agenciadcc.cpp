#include "agenciadcc.h"

AgenciaDcc::AgenciaDcc(QObject *parent) :
    QObject(parent)
{
    controladorDeRecursos = new ControladorDeRecursos();
    filaPrioritaria = new QQueue<Transacao>();
    fila = new QQueue<Transacao>();
    vetorThreadsCaixas = new QVector<ThreadCaixa*> ();
    vetorThreadsCaixas->append(new ThreadCaixa(controladorDeRecursos,fila,filaPrioritaria));
    vetorThreadsCaixas->append(new ThreadCaixa(controladorDeRecursos,fila,filaPrioritaria));
    vetorThreadsCaixas->append(new ThreadCaixa(controladorDeRecursos,fila,filaPrioritaria));
    qDebug()<<vetorThreadsCaixas->size();
    threadDafila = new ThreadFilaTransacoes(fila,controladorDeRecursos);



}
void AgenciaDcc::startThreads(){
    for (int i=0;i<3;i++){
        vetorThreadsCaixas->at(i)->start();
    }
    threadDafila->start();
}
