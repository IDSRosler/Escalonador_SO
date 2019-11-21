#include <stdlib.h>
#include <list>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Processo{
private:
    int chegada, duracao, memoria, prioridade;

public:
    Processo(int chegada, int duracao, int memoria, int prioridade){
        this->chegada    = chegada;
        this->duracao    = duracao;
        this->memoria    = memoria;
        this->prioridade = prioridade;
    }
    int getChegada(){
        return this->chegada;
    }

};

class StatusProcesso{
private:
    int chegada, lancamento, duracao_projetada, duracao_observada;
public:
    StatusProcesso(int chegada, int lancamento, int duracao_projetada, int duracao_observada){
        this->chegada = chegada;
        this->lancamento = lancamento;
        this->duracao_projetada =  duracao_projetada;
        this->duracao_observada =  duracao_observada;
    }
};

class FilaRR{
private:
    list<Processo> processos;
    int quantum = 1;    // Acredito que equivalente ao slice
    int time;
    int processoAtual;
    list<int> fila;
public:
    FilaRR(list<Processo> processos){
        this->processos = processos;
        this->time = 0;
        int pa = 1000;
        int idx = 0;
        // Como iterar sobre uma lista
        // Esse metodo so serve se a ordem do txt for modificada
        // Do jeito que esta e inutil
        list<Processo> :: iterator it;
        for(it = processos.begin(); it != processos.end(); ++it){
            if(it->getChegada() < pa){
                pa = it->getChegada();
                this->processoAtual = idx; 
            }
            idx++;
        }
        fila.push_back(processoAtual);
        this->time = 0;
    }
    int getProcessoAtual(){
        return this->processoAtual;
    }
    void tic(){
        time += quantum;
        int idx=0;
        list<Processo> :: iterator it;
        for(it = processos.begin(); it != processos.end(); ++it){
            if(idx == this->processoAtual){
                break;
            }
        }
        it->
    }  

    
};


int main(int argc, char *argv[]) {
    list<Processo> processos;
    list<StatusProcesso> sp;
    int numCPU      = atoi(argv[1]); //O número de CPUs disponíveis
    // Pinho acredita que nao precisa passar o slice
    //int slice       = atoi(argv[2]); //Duração do slice de CPU entregue a cada processo
    float sizeMem   = atof(argv[2]); //A quantidade de memória disponível (em GB)
    string nameFile = argv[3]; //O nome de um arquivo com a descrição dos processos que serão executados

    cout << "N° CPU      : " << numCPU << endl;
    // Pinho acredita que nao precisa passar slice
    //cout << "Slice       : " << slice << endl;
    cout << "Size Memory : " << sizeMem << endl;
    cout << "Nome do arq : " << nameFile << "\n" << endl;

    string line;
    int chegada, duracao, memoria, prioridade;
    ifstream processosFile (nameFile);
    if (processosFile.is_open())
    {
        getline (processosFile,line, ',');
        while (! processosFile.eof() )
        {
            chegada = stoi(line, 0, 10);
            getline (processosFile,line, ',');
            duracao = stoi(line, 0, 10);
            getline (processosFile,line, ',');
            memoria = stoi(line, 0, 10);
            getline (processosFile,line, '\n'); //erro
            prioridade = stoi(line, 0, 10);
            getline (processosFile,line, ',');

            processos.push_back(Processo(chegada, duracao, memoria, prioridade));
            sp.push_back(StatusProcesso(chegada, 0, duracao, 0));
            cout << "Chegada    : " << chegada << endl;
            cout << "Duração    : " << duracao << endl;
            cout << "Memória    : " << memoria << endl;
            cout << "Prioridade : " << prioridade << "\n" << endl;
        }
        processosFile.close();
        cout << "tam: " << processos.size() << endl;
        FilaRR fr = FilaRR(processos);
        cout << fr.getProcessoAtual() << endl;
    }
    else cout << "Unable to open file" << endl;


//    while (processosFile) {
//
//        int chegada, duracao, memoria, prioridade;
//
//        processosFile >> chegada;
//        processosFile >> duracao;
//        processosFile >> memoria;
//        processosFile >> prioridade;
//
//        Processo proc(chegada, duracao, memoria, prioridade);
//
//        processos.push_back(proc);
//    }
//
//    for (Processo proc : processos) {
//        cout << "Chegada    : " << proc.chegada << endl;
//        cout << "Duração    : " << proc.duracao << endl;
//        cout << "Memória    : " << proc.memoria << endl;
//        cout << "Prioridade : " << proc.prioridade << "\n" << endl;
//    }
}

