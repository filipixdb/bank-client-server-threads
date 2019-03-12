#include "controladorderecursos.h"


ControladorDeRecursos::ControladorDeRecursos(QObject *parent) :
    QObject(parent)
{
    //abrir arquivos de log de transacoes
    for(int j = 0; j < 4; j++)
    {
        QString nomeArquivo;

        nomeArquivo.append("ArquivoDeLog");
        nomeArquivo.append(QString::number(j));
        nomeArquivo.append(".txt");

        qDebug()<< nomeArquivo;
        vetorFileLogDeOperacao[j] = new QFile(nomeArquivo);
        vetorFileLogDeOperacao[j]->open(QIODevice::WriteOnly);

        logDeOperacao[j] = new QTextStream(vetorFileLogDeOperacao[j]);
    }
    for (int i=0; i<4; i++) vetorRecursos[i] = true;


    //abrir arquivos de clientes
    for(int j = 0; j < 4; j++)
    {
        QString nomeArquivoClientes;

        nomeArquivoClientes.append("ArquivoClientes");
        nomeArquivoClientes.append(QString::number(j));
        nomeArquivoClientes.append(".txt");

        qDebug()<< "ARQUIVO DE CLIENTES ABERTO: " << nomeArquivoClientes;
        vetorFileClientes[j] = new QFile(nomeArquivoClientes);
        // vetorFileClientes[j]->open(QIODevice::ReadWrite);

        streamClientes[j] = new QTextStream(vetorFileClientes[j]);
    }


}


bool ControladorDeRecursos::alocarRecursos(int agenciaAlfa,int agenciaBeta)
{
    mutexAlocarRecursos.lock();

    if ((vetorRecursos[agenciaAlfa]) && (vetorRecursos[agenciaBeta]))
    {
        vetorRecursos[agenciaAlfa] = false;
        vetorRecursos[agenciaBeta] = false;
        mutexAlocarRecursos.unlock();
        return true;
    } else
    {
        mutexAlocarRecursos.unlock();
        return false;
    }
}
bool ControladorDeRecursos::liberarRecursos(int agenciaAlfa,int agenciaBeta)
{
    mutexAlocarRecursos.lock();

    if ((!vetorRecursos[agenciaAlfa]) && (!vetorRecursos[agenciaBeta]))
    {
        vetorRecursos[agenciaAlfa] = true;
        vetorRecursos[agenciaBeta] = true;
        mutexAlocarRecursos.unlock();
        return true;
    } else
    {
        mutexAlocarRecursos.unlock();
        return false;
    }
}


//bool ControladorDeRecursos::efetuaTransacao(Transacao *transacao){
//    int tipoDeTransacao = transacao->tipoDeTransacao;
//    int agenciaOrigem = transacao->agenciaOrigem;
//    int contaOrigem = transacao->contaOrigem;
//    int agenciaDestino = transacao->agenciaDestino;
//    int contaDestino = transacao->contaDestino;
//    int valor = transacao->valor;
//    int tempo = transacao->tempo;

//    bool operacaoValida = true;
//    bool clienteOrigemEncontrado = false;
//    bool clienteDestinoEncontrado = false;


//    int conta, agencia, saldo;
//    int conta2, agencia2, saldo2;
//    QString nome, nome2,linha;
//    QString linhaApontada, linhaApontada2;
//    QStringList camposDaLinhaApontada,camposDaLinhaApontada2,linhasDoArquivo,linhasDoArquivo2;

//    int posicaoClienteOrigem;





//    if (tipoDeTransacao == 0){//deposito
//    }else if (tipoDeTransacao == 1){//saque

//        vetorFileClientes[agenciaOrigem]->open(QIODevice::ReadOnly);
//        linhasDoArquivo.clear();
//        camposDaLinhaApontada.clear();

//        do{
//            linhasDoArquivo.append(streamClientes[agenciaOrigem]->readLine());
//            camposDaLinhaApontada = linhasDoArquivo.last().split(":");

//            conta = (camposDaLinhaApontada.at(0)).toInt();
//            agencia = (camposDaLinhaApontada.at(1)).toInt();
//            nome = camposDaLinhaApontada.at(2);
//            saldo = (camposDaLinhaApontada.at(3)).toInt();


//        }while ((conta != contaOrigem) && !(streamClientes[agenciaOrigem]->atEnd()));

//        if (conta == contaOrigem){//achei o cliente

//            clienteOrigemEncontrado = true;
//            //testar se o cliente origem tem saldo
//            if (saldo < valor) operacaoValida = false;

//        }else {//nao achei o cliente origem
//            clienteOrigemEncontrado = false;

//        }

//        if (operacaoValida && clienteOrigemEncontrado){
//            //executar a operacao

//            QString linhaAtualizada = camposDaLinhaApontada.at(0) + ":" + camposDaLinhaApontada.at(1) + ":" + camposDaLinhaApontada.at(2) + ":" + QString::number(saldo - valor) ;
//            linhasDoArquivo.last() = linhaAtualizada;
//            while (!streamClientes[agenciaOrigem]->atEnd()){
//                linhasDoArquivo.append(streamClientes[agenciaOrigem]->readLine());
//            }

//        }else {
//            qDebug() << "Problemas com a transferencia";
//            while (!streamClientes[agenciaOrigem]->atEnd()){
//                streamClientes[agenciaOrigem]->readLine();
//            }
//        }

//        vetorFileClientes[agenciaOrigem]->close();


//        if (operacaoValida && clienteOrigemEncontrado){

//            vetorMutexClientes[agenciaOrigem].lock();
//            vetorFileClientes[agenciaOrigem]->open(QIODevice::WriteOnly);

//            for (int k = 0; k < linhasDoArquivo.size(); k++){
//                //qDebug() << "uma das linhas escrotas: " << linhasDoArquivo.at(k) <<endl;
//                *streamClientes[agenciaOrigem] << linhasDoArquivo.at(k) << endl;
//            }

//            vetorFileClientes[agenciaOrigem]->close();

//        }

//    }else if (tipoDeTransacao == 2){//transferencia

//        if(agenciaOrigem!=agenciaDestino){//tranferencia entre agencias diferentes

//            vetorFileClientes[agenciaOrigem]->open(QIODevice::ReadOnly);
//            vetorFileClientes[agenciaDestino]->open(QIODevice::ReadOnly);

//            linhasDoArquivo.clear();
//            camposDaLinhaApontada.clear();

//            do{ //buscar cliente origem


//                linhasDoArquivo.append(streamClientes[agenciaOrigem]->readLine());

//                camposDaLinhaApontada = linhasDoArquivo.last().split(":");

//                conta = (camposDaLinhaApontada.at(0)).toInt();
//                agencia = (camposDaLinhaApontada.at(1)).toInt();
//                nome = camposDaLinhaApontada.at(2);
//                saldo = (camposDaLinhaApontada.at(3)).toInt();


//            }while ((conta != contaOrigem) && !(streamClientes[agenciaOrigem]->atEnd()));



//            if (conta == contaOrigem){//achei o cliente

//                clienteOrigemEncontrado = true;
//                //testar se o cliente origem tem saldo
//                if (saldo < valor) operacaoValida = false;

//            }else {//nao achei o cliente origem
//                clienteOrigemEncontrado = false;

//            }



//            linhasDoArquivo2.clear();
//            camposDaLinhaApontada2.clear();

//            do{ //buscar cliente destino


//                linhasDoArquivo2.append(streamClientes[agenciaDestino]->readLine());

//                camposDaLinhaApontada2 = linhasDoArquivo2.last().split(":");

//                conta2 = (camposDaLinhaApontada2.at(0)).toInt();
//                agencia2 = (camposDaLinhaApontada2.at(1)).toInt();
//                nome2 = camposDaLinhaApontada2.at(2);
//                saldo2 = (camposDaLinhaApontada2.at(3)).toInt();


//            }while ((conta2 != contaDestino) && !(streamClientes[agenciaDestino]->atEnd()));

//            if (conta2 == contaDestino){//achei o cliente destino
//                clienteDestinoEncontrado = true;
//            }  else {//nao achei o cliente destino
//                clienteDestinoEncontrado = false;
//            }


//            if (operacaoValida && clienteOrigemEncontrado && clienteDestinoEncontrado){
//                //executar a operacao


//                QString linhaAtualizada = camposDaLinhaApontada.at(0) + ":" + camposDaLinhaApontada.at(1) + ":" + camposDaLinhaApontada.at(2) + ":" + QString::number(saldo - valor) ;
//                linhasDoArquivo.last() = linhaAtualizada;
//                while (!streamClientes[agenciaOrigem]->atEnd()){
//                    linhasDoArquivo.append(streamClientes[agenciaOrigem]->readLine());
//                }


//                QString linhaAtualizada2 = camposDaLinhaApontada2.at(0) + ":" + camposDaLinhaApontada2.at(1) + ":" + camposDaLinhaApontada2.at(2) + ":" + QString::number(saldo2 + valor) ;
//                linhasDoArquivo2.last() = linhaAtualizada2;
//                while (!streamClientes[agenciaDestino]->atEnd()){
//                    linhasDoArquivo2.append(streamClientes[agenciaDestino]->readLine());
//                }


//            }else {
//                qDebug() << "Problemas com a transferencia";

//                while (!streamClientes[agenciaOrigem]->atEnd()){
//                    linhasDoArquivo.append(streamClientes[agenciaOrigem]->readLine());
//                }


//                while (!streamClientes[agenciaDestino]->atEnd()){
//                    linhasDoArquivo2.append(streamClientes[agenciaDestino]->readLine());
//                }


//            }




//            vetorFileClientes[agenciaOrigem]->close();
//            vetorFileClientes[agenciaDestino]->close();


//            if (operacaoValida && clienteOrigemEncontrado && clienteDestinoEncontrado){

//                vetorMutexClientes[agenciaOrigem].lock();
//                vetorFileClientes[agenciaOrigem]->open(QIODevice::WriteOnly);
//                vetorMutexClientes[agenciaDestino].lock();
//                vetorFileClientes[agenciaDestino]->open(QIODevice::WriteOnly);




//                for (int k = 0; k < linhasDoArquivo.size(); k++){
//                    //qDebug() << "uma das linhas escrotas: " << linhasDoArquivo.at(k) <<endl;
//                    *streamClientes[agenciaOrigem] << linhasDoArquivo.at(k) << endl;
//                }


//                for (int m = 0; m < linhasDoArquivo2.size(); m++){
//                    //qDebug() << "uma das linhas escrotas: " << linhasDoArquivo.at(k) <<endl;
//                    *streamClientes[agenciaDestino] << linhasDoArquivo2.at(m) << endl;
//                }


//                vetorFileClientes[agenciaOrigem]->close();
//                vetorFileClientes[agenciaDestino]->close();


//                vetorMutexClientes[agenciaOrigem].unlock();
//                vetorMutexClientes[agenciaDestino].unlock();


//            }



//        }
//        else{//tranferencia entre agencias iguais


//            vetorFileClientes[agenciaOrigem]->open(QIODevice::ReadOnly);

//            //testar se as contas sao diferentes
//            if (contaOrigem == contaDestino) operacaoValida = false;

//            linhasDoArquivo.clear();
//            camposDaLinhaApontada.clear();

//            do{ //buscar cliente origem


//                linhasDoArquivo.append(streamClientes[agenciaOrigem]->readLine());

//                camposDaLinhaApontada = linhasDoArquivo.last().split(":");

//                conta = (camposDaLinhaApontada.at(0)).toInt();
//                agencia = (camposDaLinhaApontada.at(1)).toInt();
//                nome = camposDaLinhaApontada.at(2);
//                saldo = (camposDaLinhaApontada.at(3)).toInt();


//            }while ((conta != contaOrigem) && !(streamClientes[agenciaOrigem]->atEnd()));



//            if (conta == contaOrigem){//achei o cliente

//                clienteOrigemEncontrado = true;
//                posicaoClienteOrigem = linhasDoArquivo.size() - 1;
//                //testar se o cliente origem tem saldo
//                if (saldo < valor) operacaoValida = false;

//            }else {//nao achei o cliente origem
//                clienteOrigemEncontrado = false;

//            }
//            //até aqui agente achou o primeiro e viu que ele tem saldo e agente tem o lock da agencia
//            // as linhas do arquivo contem todas as linhas até o usuario origem

//            while (!streamClientes[agenciaOrigem]->atEnd()){
//                linhasDoArquivo.append(streamClientes[agenciaOrigem]->readLine());
//            }



//            if (clienteOrigemEncontrado == true){

//                for(int ro=0;ro<linhasDoArquivo.size();ro++){ //procurando o cliente destino

//                    linha = linhasDoArquivo.at(ro);
//                    camposDaLinhaApontada2 = linha.split(":");

//                    conta2 = (camposDaLinhaApontada2.at(0)).toInt();

//                    clienteDestinoEncontrado = false;

//                    if (conta2 == contaDestino){

//                        agencia2 = (camposDaLinhaApontada2.at(1)).toInt();
//                        nome2 = camposDaLinhaApontada2.at(2);
//                        saldo2 = (camposDaLinhaApontada2.at(3)).toInt();

//                        //alterando o saldo do cliente Destino
//                        saldo2 += valor;

//                        linha = camposDaLinhaApontada2.at(0) + ":" + camposDaLinhaApontada2.at(1) + ":" + camposDaLinhaApontada2.at(2) + ":" + QString::number(saldo2);

//                        linhasDoArquivo.replace(ro, linha);

//                        clienteDestinoEncontrado = true;

//                        break;

//                    }

//                }
//            }



//            if (clienteDestinoEncontrado == true){


//                linha = linhasDoArquivo.at(posicaoClienteOrigem);
//                camposDaLinhaApontada = linha.split(":");

//                conta = (camposDaLinhaApontada.at(0)).toInt();

//                agencia = (camposDaLinhaApontada.at(1)).toInt();
//                nome = camposDaLinhaApontada.at(2);
//                saldo = (camposDaLinhaApontada.at(3)).toInt();

//                //alterando o saldo do cliente Origem
//                saldo -= valor;

//                linha = camposDaLinhaApontada.at(0) + ":" + camposDaLinhaApontada.at(1) + ":" + camposDaLinhaApontada.at(2) + ":" + QString::number(saldo);

//                linhasDoArquivo.replace(posicaoClienteOrigem, linha);


//            }

//            //fecha o arquivo para leitura

//            vetorFileClientes[agenciaOrigem]->close();


//            if (operacaoValida && clienteOrigemEncontrado && clienteDestinoEncontrado){

//                vetorMutexClientes[agenciaOrigem].lock();
//                vetorFileClientes[agenciaOrigem]->open(QIODevice::WriteOnly);



//                for (int k = 0; k < linhasDoArquivo.size(); k++){
//                    //qDebug() << "uma das linhas escrotas: " << linhasDoArquivo.at(k) <<endl;
//                    *streamClientes[agenciaOrigem] << linhasDoArquivo.at(k) << endl;
//                }


//                vetorFileClientes[agenciaOrigem]->close();

//                vetorMutexClientes[agenciaOrigem].unlock();


//            }
//            else{
//                qDebug() << "Problemas com a transferencia em uma mesma agencia";

//                while (!streamClientes[agenciaOrigem]->atEnd()){
//                    linhasDoArquivo.append(streamClientes[agenciaOrigem]->readLine());
//                }


//            }


//        }



//    }

//}
ControladorDeRecursos::~ControladorDeRecursos(){
    for(int i=0;i<4;i++){
        vetorFileLogDeOperacao[i]->close();
        vetorFileClientes[i]->close();
    }
}
