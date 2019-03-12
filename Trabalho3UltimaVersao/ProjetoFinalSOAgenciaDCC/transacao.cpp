#include "transacao.h"

Transacao::Transacao(int tipoDeTransacao, int agenciaOrigem, int contaOrigem, int agenciaDestino, int contaDestino, int valor, int tempo){
    this-> tipoDeTransacao = tipoDeTransacao;
    this->agenciaOrigem = agenciaOrigem;
    this->contaOrigem = contaOrigem;
    this->agenciaDestino = agenciaDestino;
    this->contaDestino = contaDestino;
    this->valor = valor;
    this->tempo = tempo;
}
Transacao::Transacao(){

}
