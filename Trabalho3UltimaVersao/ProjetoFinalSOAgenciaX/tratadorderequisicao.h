#ifndef TRATADORDEREQUISICAO_H
#define TRATADORDEREQUISICAO_H
#include <qt4/Qt/qtcpserver.h>
#include <QObject>
#include <qt4/Qt/qtcpsocket.h>
#include <QFile>
#include <QTextStream>
#include <QStringList>

class TratadorDeRequisicao : public QObject
{
    Q_OBJECT
public:
    explicit TratadorDeRequisicao(QObject *parent = 0);

    ~TratadorDeRequisicao();
    QTcpServer * servidor;
    QTcpSocket *clientConnection;

    QFile * arquivoAgencia;
    QFile * arquivoPorta;
    QFile * arquivoClientes;
    QFile * arquivoLog;

    QTextStream * agenciaStream ;
    QTextStream * portaStream;
    QTextStream * clientesStream;
    QTextStream * logStream;

    QStringList contaAgenciaSeparadas;

    void iniciaServer();
    QString saldo(QString contaAgencia);

    QString numeroPorta,idAgencia;


signals:

public slots:
    void novocliente();

};

#endif // TRATADORDEREQUISICAO_H
