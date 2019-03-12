#include "tratadorderequisicao.h"
TratadorDeRequisicao::~TratadorDeRequisicao(){
    arquivoAgencia->close();
    arquivoPorta->close();
    arquivoClientes->close();
    arquivoLog->close();
}

TratadorDeRequisicao::TratadorDeRequisicao(QObject *parent) :
    QObject(parent)
{
    servidor = new QTcpServer(this);


    arquivoAgencia=new QFile("agencia.txt");
    arquivoPorta=new QFile("porta.txt");
    arquivoLog=new QFile("log.txt");

    arquivoAgencia->open(QIODevice::ReadOnly);
    arquivoPorta->open(QIODevice::ReadOnly);
    arquivoLog->open(QIODevice::WriteOnly);


    agenciaStream = new QTextStream(arquivoAgencia);
    portaStream = new QTextStream(arquivoPorta);
    logStream = new QTextStream(arquivoLog);



    *agenciaStream >> this->idAgencia;
    *portaStream >> this->numeroPorta;

    QString nomeArquivoCliente;
    nomeArquivoCliente.append("ArquivoClientes");
    nomeArquivoCliente.append(idAgencia);
    nomeArquivoCliente.append(".txt");

    arquivoClientes = new QFile(nomeArquivoCliente);
    // arquivoClientes->open(QIODevice::ReadOnly);

    clientesStream = new QTextStream(arquivoClientes);

    iniciaServer();

    qDebug()<<idAgencia<<endl<<numeroPorta;

}
void TratadorDeRequisicao::iniciaServer(){


    qDebug()<<idAgencia.toInt();
    if (!servidor->listen(QHostAddress::Any, (qint16)5544+idAgencia.toInt())) {
        qDebug()<<"Servidor não está escutando";

    }

    *logStream<<"Servidor Iniciado escutando a porta";
    *logStream<<QString::number((qint16)5544+idAgencia.toInt())<<endl;
    connect(servidor, SIGNAL(newConnection()), this, SLOT(novocliente()));
    qDebug()<<"iniciou servidor";
}
void TratadorDeRequisicao::novocliente(){
    clientConnection =  servidor->nextPendingConnection();
    *logStream<<"cliente conectado"<<endl;
    *logStream<<"\tesperando receber os dados"<<endl;
    clientConnection->waitForReadyRead(1000);
    QString recebido= QString(clientConnection->readAll());
    qDebug()<<"recebido pelo servidor"<< recebido;
    *logStream<<"\trecebeu a mensagem "<<recebido<<endl;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint16)0;

    out <<saldo(recebido);
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    *logStream<<"\tenviou "<<block<<" para o cliente";
    clientConnection->write(block);





}
QString TratadorDeRequisicao::saldo(QString contaAgencia){

    if(contaAgencia.count(":")>0){
        bool clienteOrigemEncontrado = false;



        int conta, agencia, saldo;
        QString nome;
        QStringList camposDaLinhaApontada,linhasDoArquivo;

        //checa agencia certa

        //minhas variaveis
        contaAgenciaSeparadas=contaAgencia.split(":");


        arquivoClientes->open(QIODevice::ReadOnly);

        //testar se as contas sao diferentes


        linhasDoArquivo.clear();
        camposDaLinhaApontada.clear();

        do{ //buscar cliente origem


            linhasDoArquivo.append(clientesStream->readLine());

            camposDaLinhaApontada = linhasDoArquivo.last().split(":");

            conta = (camposDaLinhaApontada.at(0)).toInt();
            agencia = (camposDaLinhaApontada.at(1)).toInt();
            nome = camposDaLinhaApontada.at(2);
            saldo = (camposDaLinhaApontada.at(3)).toInt();


        }while ((conta != contaAgenciaSeparadas.at(0).toInt()) && !(clientesStream->atEnd()));



        if (conta == contaAgenciaSeparadas.at(0).toInt()){//achei o cliente

            clienteOrigemEncontrado = true;

        }else {//nao achei o cliente origem
            clienteOrigemEncontrado = false;

        }

        while (!clientesStream->atEnd()){
            linhasDoArquivo.append(clientesStream->readLine());
        }


        arquivoClientes->close();



        if (clienteOrigemEncontrado) return QString::number(saldo);
        else{
            return QString("Conta nao encontrada");
        }
    }else{

        qDebug() << "Me pediu para atualizar o saldo: " << contaAgencia;

        //procurar por essa conta agencia
        if(contaAgenciaSeparadas.size()==0){
            qDebug()<<"problemas com thread";
            return QString("falha");
        }
        bool clienteOrigemEncontrado = false;

        int ro=0;
        int conta, agencia, saldo;
        QString nome;
        QString linhaAtualizada;
        QStringList camposDaLinhaApontada,linhasDoArquivo;


        arquivoClientes->open(QIODevice::ReadOnly);

        //testar se as contas sao diferentes


        linhasDoArquivo.clear();
        camposDaLinhaApontada.clear();

        do{ //buscar cliente origem


            linhasDoArquivo.append(clientesStream->readLine());

            camposDaLinhaApontada = linhasDoArquivo.last().split(":");

            conta = (camposDaLinhaApontada.at(0)).toInt();
            agencia = (camposDaLinhaApontada.at(1)).toInt();
            nome = camposDaLinhaApontada.at(2);
            saldo = (camposDaLinhaApontada.at(3)).toInt();


        }while ((conta != contaAgenciaSeparadas.at(0).toInt()) && !(clientesStream->atEnd()));



        if (conta == contaAgenciaSeparadas.at(0).toInt()){//achei o cliente

            clienteOrigemEncontrado = true;
            ro = linhasDoArquivo.size()-1;


        }else {//nao achei o cliente origem
            clienteOrigemEncontrado = false;
            while (!clientesStream->atEnd()){
                linhasDoArquivo.append(clientesStream->readLine());
            }
            arquivoClientes->close();
            return QString("cliente não encontrado");
        }


        if(clienteOrigemEncontrado){
            linhaAtualizada = QString::number(conta) + ":" + QString::number(agencia)+ ":" + nome + ":" + contaAgencia;
            linhasDoArquivo.last()=linhaAtualizada;

        }



        while (!clientesStream->atEnd()){
            linhasDoArquivo.append(clientesStream->readLine());
        }


        arquivoClientes->close();
        arquivoClientes->open(QIODevice::WriteOnly);

        for (int k = 0; k < linhasDoArquivo.size(); k++){
            //qDebug() << "uma das linhas escrotas: " << linhasDoArquivo.at(k) <<endl;
            *clientesStream << linhasDoArquivo.at(k) << endl;
        }
        arquivoClientes->close();
        this->contaAgenciaSeparadas.clear();
        return QString("sucesso");



        //retorna sucesso


    }

}
