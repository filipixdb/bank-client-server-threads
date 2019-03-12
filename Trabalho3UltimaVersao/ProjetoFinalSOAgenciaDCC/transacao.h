#ifndef TRANSACAO_H
#define TRANSACAO_H


class Transacao
{

public:

    Transacao(int tipoDeTransacao, int agenciaOrigem, int contaOrigem, int agenciaDestino, int contaDestino, int valor, int tempo);
    Transacao();
    /** Variavel para o tipo de transacao
      */
    int tipoDeTransacao;

    /** Variavel para a agencia de origem da transacao
      */
    int agenciaOrigem;

    /** Variavel para a conta de origem da transacao
      */
    int contaOrigem;

    /** Variavel para agencia de destino da transacao
      */
    int agenciaDestino;

    /** Variavel para conta de destino da transacao
      */
    int contaDestino;

    /** Variavel de valor da transacao
      */
    int valor;

    /** Variavel do tempo em que chegou a transacao
      */
    int tempo;


};

#endif // TRANSACAO_H
