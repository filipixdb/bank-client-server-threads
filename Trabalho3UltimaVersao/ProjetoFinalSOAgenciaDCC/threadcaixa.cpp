#include "threadcaixa.h"

ThreadCaixa::ThreadCaixa(QObject *parent) :
    QThread(parent)
{
    // this->start();
}


void ThreadCaixa::run()
{
    Transacao atual;
    int i=0;
    int agenciaAlfa,agenciaBeta;
    while(true){


        controladorDeRecursos->mutexFilaPrioritaria.lock();
        if(filaPrioritaria->count()>0){
            atual = filaPrioritaria->dequeue();
            controladorDeRecursos->mutexFilaPrioritaria.unlock();
            agenciaAlfa = atual.agenciaOrigem;
            agenciaBeta = atual.agenciaDestino;
            //qDebug()<<"usando fila prioritária";
        }
        else{
            controladorDeRecursos->mutexFilaPrioritaria.unlock();
            controladorDeRecursos->mutexFila.lock();

            if(fila->count()>0){
                atual = fila->dequeue();
                controladorDeRecursos->mutexFila.unlock();
                agenciaAlfa = atual.agenciaOrigem;
                agenciaBeta = atual.agenciaDestino;
                //  qDebug()<<"Olhando a transacao na fila, agenciaAlfa="<< agenciaAlfa <<" e agenciaBeta=" << agenciaBeta;


            }
            else {
                controladorDeRecursos->mutexFila.unlock();
                // sleep(0.1);
                this->yieldCurrentThread();
                continue;
            } // Fila principal vazia.


        }
        bool alocado = controladorDeRecursos->alocarRecursos(agenciaAlfa,agenciaBeta);
        if(alocado)
        {
            //realizarTransacao();

            this->setPriority(QThread::HighestPriority);

            realizarTransacao(&atual);
            controladorDeRecursos->vetorMutexLog[3].lock();
            *controladorDeRecursos->logDeOperacao[3]<<"Tratando a transação "<<atual.agenciaOrigem<<" ag origem "<<atual.contaOrigem<<" conta origem";
            *controladorDeRecursos->logDeOperacao[3]<<" "<<atual.agenciaDestino<<" ag destino "<<atual.contaDestino<<" conta destino"<<endl;
            *controladorDeRecursos->logDeOperacao[3]<<"Tipo de operação" <<atual.tipoDeTransacao<<"tipo de operação " << atual.valor <<" valor";

            controladorDeRecursos->vetorMutexLog[3].unlock();
            //efetuaTransacao(&atual);

            /*            controladorDeRecursos->vetorMutexLog[agenciaAlfa].lock();
            *controladorDeRecursos->logDeOperacao[agenciaAlfa] << "Aloquei recursos" << QString::number(agenciaAlfa) << " e " << QString::number(agenciaBeta)<<endl;
            controladorDeRecursos->vetorMutexLog[agenciaAlfa].unlock();

            controladorDeRecursos->vetorMutexLog[agenciaBeta].lock();
            *controladorDeRecursos->logDeOperacao[agenciaBeta] << "Aloquei recursos" << QString::number(agenciaAlfa) << " e " << QString::number(agenciaBeta)<<endl;
            controladorDeRecursos->vetorMutexLog[agenciaBeta].unlock();*/
            // controladorDeRecursos->vetorFileLogDeOperacao[agenciaAlfa]->close();
            // controladorDeRecursos->vetorFileLogDeOperacao[agenciaBeta]->close();
            // sleep(1);
            bool liberou = controladorDeRecursos->liberarRecursos(agenciaAlfa,agenciaBeta);
            if (liberou == false){
                // qDebug()<<"Não pude liberar recurso";
            }
            this->setPriority(QThread::NormalPriority);

            // qDebug()<<i;
            i++;
            // qDebug() << (QString)"liberei recursos " << agenciaAlfa << (QString)" e " << agenciaBeta;

        } else
        {
            //qDebug() << "NOT Aloquei recursos " << agenciaAlfa << " e " << agenciaBeta;
            //filaPrioritaria->enqueue(new Transacao(0,agenciaAlfa,0,agenciaBeta,0,101,1));
            controladorDeRecursos->mutexFilaPrioritaria.lock();
            filaPrioritaria->enqueue(atual);
            controladorDeRecursos->mutexFilaPrioritaria.unlock();
        }


    }
}

ThreadCaixa::ThreadCaixa(ControladorDeRecursos* controladorDeRecursos,QQueue<Transacao> * fila, QQueue<Transacao> * filaPrioritaria)
{

    this->controladorDeRecursos = controladorDeRecursos;
    this->fila = fila;
    this->filaPrioritaria=filaPrioritaria;
    this->chequeEspecial=100;
}

QString ThreadCaixa::comunicaAgenciaX(int x,QString mensagem){

    const int Timeout = 5 * 1000;

    QTcpSocket socket;

    socket.connectToHost("127.0.0.1",(qint16)5544+x);

    if (!socket.waitForConnected(Timeout)) {
        qDebug()<<"erro no socket";
        return QString("Erro na conexão com o server");
    }

    socket.write(mensagem.toUtf8());
    socket.waitForBytesWritten(Timeout);
    socket.flush();


    while (socket.bytesAvailable() < (int)sizeof(quint16)) {
        if (!socket.waitForReadyRead(Timeout)) {
            qDebug()<<"erro no socket";
            return QString("Erro na leitura da resposta do socket");
        }
    }

    quint16 blockSize;
    QDataStream in(&socket);
    in.setVersion(QDataStream::Qt_4_6);
    in >> blockSize;
    //qDebug()<<"numero de bytes"<<blockSize;
    while (socket.bytesAvailable() < blockSize) {
        if (!socket.waitForReadyRead(Timeout)) {
            qDebug()<<"erro no socket";
            return QString("erro esperando o block todo");
        }
    }

    QString fortune;
    in >> fortune;
    return fortune;



}

bool ThreadCaixa::realizarTransacao(Transacao * transacao){

    //testar o tipo de transacao

    if (transacao->tipoDeTransacao == 0){//deposito



        QString contaAgencia, contaAgenciaSaldo;
        QString respostaSaldo = "Conta nao encontrada";

        bool clienteOrigemEncontrado = false;

        contaAgencia.append(QString::number(transacao->contaOrigem));
        contaAgencia.append(":");
        contaAgencia.append(QString::number(transacao->agenciaOrigem));


        qDebug() << "string conta agencia" << contaAgencia;


        respostaSaldo = comunicaAgenciaX(transacao->agenciaOrigem,contaAgencia);

        if(!respostaSaldo.toInt()){
            qDebug()<<"Conta não encontrada no servidor";
            return false;
        }
        else{

            clienteOrigemEncontrado = true;

            qDebug()<<respostaSaldo;
        }


        //calcula o novo saldo

        contaAgenciaSaldo = "";
        contaAgenciaSaldo.append(QString::number(respostaSaldo.toInt() + transacao->valor));


        qDebug() << "Novos dados " << contaAgenciaSaldo;

        comunicaAgenciaX(transacao->agenciaOrigem,contaAgenciaSaldo);









    }else if(transacao->tipoDeTransacao == 1){//saque


        QString contaAgencia, contaAgenciaSaldo;
        QString respostaSaldo="Conta nao encontrada";

        bool clienteOrigemEncontrado = false;

        contaAgencia.append(QString::number(transacao->contaOrigem));
        contaAgencia.append(":");
        contaAgencia.append(QString::number(transacao->agenciaOrigem));


        qDebug() << "string conta agencia" << contaAgencia;


        respostaSaldo = comunicaAgenciaX(transacao->agenciaOrigem,contaAgencia);

        if(!respostaSaldo.toInt()){
            qDebug()<<"Conta não encontrada no servidor";
            return false;

        }
        else{

            clienteOrigemEncontrado = true;

            qDebug()<<respostaSaldo;
        }
        //verifica saldo


        if((respostaSaldo.toInt()+this->chequeEspecial) >= transacao->valor){

            //calcula o novo saldo

            contaAgenciaSaldo = "";
            contaAgenciaSaldo.append(QString::number(respostaSaldo.toInt() - transacao->valor));


            qDebug() << "Novos dados " << contaAgenciaSaldo;

            comunicaAgenciaX(transacao->agenciaOrigem,contaAgenciaSaldo);
        }







    }else if(transacao->tipoDeTransacao == 2){//transferencia


        QString respostaSaldo1 = "Conta nao encontrada";
        QString respostaSaldo2 = "Conta nao encontrada";
        QString contaAgencia1, contaAgencia2;
        QString contaOrigem, contaDestino;
        QString agenciaOrigem, agenciaDestino;
        QString novoSaldo1, novoSaldo2;

        bool clienteOrigemEncontrado = false;
        bool clienteDestinoEncontrado = false;
        bool operacaoValida = true;


        if (transacao->agenciaOrigem == transacao->agenciaDestino){//transferencia entre agencias iguais



            //pegar os dados da transacao

            //buscar o primeiro cliente

            //verifica se o cliente tem saldo

            //


            //pega os dados da transacao

            contaOrigem = QString::number(transacao->contaOrigem);
            contaDestino = QString::number(transacao->contaDestino);
            agenciaOrigem = QString::number(transacao->agenciaOrigem);
            agenciaDestino = QString::number(transacao->agenciaDestino);



            qDebug() << "DADOS DA TRANSFERENCIA ENTRE AGENCIAS DIFERENTES:" << endl << contaOrigem << endl << contaDestino << endl << agenciaOrigem << endl << agenciaDestino <<endl << "--------------" << endl;


            //buscar o primeiro cliente
            contaAgencia1 = contaOrigem + ":" + agenciaOrigem;

            qDebug() << "contaAgencia do cliente 1 da transacao: " << contaAgencia1 <<endl;

            qDebug() << "VALOR DO X " << agenciaOrigem.toInt()+98000;

            respostaSaldo1 = comunicaAgenciaX(transacao->agenciaOrigem, contaAgencia1);


            if (!respostaSaldo1.toInt()){
                qDebug()<<"Conta cliente 1 transferencia não encontrada no servidor";
                return false;

            }
            else{

                clienteOrigemEncontrado = true;

                qDebug() << "Saldo do cliente 1 transferencia: " << respostaSaldo1 << endl;
            }


            //verifica se o cliente tem saldo

            if ((respostaSaldo1.toInt()+ this->chequeEspecial) >= transacao->valor){//cliente possui valor

                //buscar cliente2

                //buscar o segundo cliente
                contaAgencia2 = contaDestino + ":" + agenciaDestino;

                qDebug() << "contaAgencia do cliente 2 da transacao: " << contaAgencia2 <<endl;

                //pegar saldo cliente 2

                respostaSaldo2 = comunicaAgenciaX(agenciaDestino.toInt(), contaAgencia2);


                if (!respostaSaldo2.toInt()){
                    qDebug()<<"Conta cliente 2 transferencia não encontrada no servidor";
                    return false;

                }
                else{

                    clienteDestinoEncontrado = true;

                    qDebug() << "Saldo do cliente 2 transferencia: " << respostaSaldo2 << endl;
                }



                //somar valor da transacao cliente 2

                novoSaldo2 = QString::number(respostaSaldo2.toInt() + transacao->valor);


                //verificar resposta do servidor

                qDebug() << "Pedido para alterar o saldo cliente 2 para: " << novoSaldo2 << endl;

                respostaSaldo2 = comunicaAgenciaX(transacao->agenciaDestino, novoSaldo2);

                if (!(respostaSaldo2 == "sucesso")){
                    qDebug()<<"Saldo cliente 2 transferencia não atualizado no servidor";
                    return false;

                }
                else{//atualizou com sucesso

                    qDebug() << "Saldo do cliente 2 transferencia atualizado com sucesso: " << endl;
                }


                //retirar valor da transacao cliente 1

                novoSaldo1 = QString::number(respostaSaldo1.toInt() - transacao->valor);


                //verificar resposta do servidor

                qDebug() << "Pedido para alterar o saldo cliente 1 para: " << novoSaldo1 << endl;

                respostaSaldo1 = comunicaAgenciaX(transacao->agenciaOrigem, novoSaldo1);


                //verificar resposta do servidor
                if (!(respostaSaldo2 == "sucesso")){
                    qDebug()<<"Saldo cliente 1 transferencia não atualizado no servidor";

                    //se deu problema aqui, devo desfazer o deposito pro cliente 2;

                    return false;

                }
                else{//atualizou com sucesso

                    qDebug() << "Saldo do cliente 1 transferencia atualizado com sucesso: " << endl;
                }



                return true;



            }else {//cliente nao possui valor
                qDebug() << "ERRO Cliente 1 nao possui saldo...";
                return false;

            }




        }else {//transferencia entre agencias diferentes


            //pega os dados da transacao

            contaOrigem = QString::number(transacao->contaOrigem);
            contaDestino = QString::number(transacao->contaDestino);
            agenciaOrigem = QString::number(transacao->agenciaOrigem);
            agenciaDestino = QString::number(transacao->agenciaDestino);



            qDebug() << "DADOS DA TRANSFERENCIA ENTRE AGENCIAS DIFERENTES:" << endl << contaOrigem << endl << contaDestino << endl << agenciaOrigem << endl << agenciaDestino <<endl << "--------------" << endl;


            //buscar o primeiro cliente
            contaAgencia1 = contaOrigem + ":" + agenciaOrigem;

            qDebug() << "contaAgencia do cliente 1 da transacao: " << contaAgencia1 <<endl;

            qDebug() << "VALOR DO X " << agenciaOrigem.toInt()+98000;

            respostaSaldo1 = comunicaAgenciaX(transacao->agenciaOrigem, contaAgencia1);


            //testa se encontrados os dados

            if (!respostaSaldo1.toInt()){
                qDebug()<<"Conta cliente 1 transferencia não encontrada no servidor";
                return false;

            }
            else{

                clienteOrigemEncontrado = true;

                qDebug() << "Saldo do cliente 1 transferencia: " << respostaSaldo1 << endl;
            }


            //verifica se o cliente tem saldo

            if ((respostaSaldo1.toInt()+ this->chequeEspecial) >= transacao->valor){//cliente possui valor

                //buscar cliente2

                //buscar o segundo cliente
                contaAgencia2 = contaDestino + ":" + agenciaDestino;

                qDebug() << "contaAgencia do cliente 2 da transacao: " << contaAgencia2 <<endl;

                //pegar saldo cliente 2

                respostaSaldo2 = comunicaAgenciaX(agenciaDestino.toInt(), contaAgencia2);




                if (!respostaSaldo2.toInt()){
                    qDebug()<<"Conta cliente 2 transferencia não encontrada no servidor";
                    return false;

                }
                else{

                    clienteDestinoEncontrado = true;

                    qDebug() << "Saldo do cliente 2 transferencia: " << respostaSaldo2 << endl;
                }



                //somar valor da transacao cliente 2

                novoSaldo2 = QString::number(respostaSaldo2.toInt() + transacao->valor);


                //verificar resposta do servidor

                qDebug() << "Pedido para alterar o saldo cliente 2 para: " << novoSaldo2 << endl;

                respostaSaldo2 = comunicaAgenciaX(transacao->agenciaDestino, novoSaldo2);

                if (!(respostaSaldo2 == "sucesso")){
                    qDebug()<<"Saldo cliente 2 transferencia não atualizado no servidor";
                    return false;

                }
                else{//atualizou com sucesso

                    qDebug() << "Saldo do cliente 2 transferencia atualizado com sucesso: " << endl;
                }


                //retirar valor da transacao cliente 1

                novoSaldo1 = QString::number(respostaSaldo1.toInt() - transacao->valor);


                //verificar resposta do servidor

                qDebug() << "Pedido para alterar o saldo cliente 1 para: " << novoSaldo1 << endl;

                respostaSaldo1 = comunicaAgenciaX(transacao->agenciaOrigem, novoSaldo1);


                //verificar resposta do servidor
                if (!(respostaSaldo1 == "sucesso")){
                    qDebug()<<"Saldo cliente 1 transferencia não atualizado no servidor";

                    //se deu problema aqui, devo desfazer o deposito pro cliente 2;

                    return false;

                }
                else{//atualizou com sucesso

                    qDebug() << "Saldo do cliente 1 transferencia atualizado com sucesso: " << endl;
                }



                return true;



            }else {//cliente nao possui valor
                qDebug() << "ERRO Cliente 1 nao possui saldo...";
                return false;

            }





        }




    }

}
