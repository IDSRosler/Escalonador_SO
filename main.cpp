#include <stdlib.h>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

class Processo{
private:
    int chegada, duracao, memoria, prioridade, lancamento, duracao_observada, duracao_projetada, prioridade_atual, pid, decPrioridade;

public:
    Processo(int chegada, int duracao, int memoria, int prioridade, int pid){
        this->chegada    = chegada;
        this->duracao    = duracao;
        this->memoria    = memoria;
        this->prioridade = prioridade;
        this->duracao_projetada = duracao;
        this->prioridade_atual = prioridade;
        this->pid = pid;
        this->decPrioridade = 1;
    }
    // Gets
    int getChegada(){
        return this->chegada;
    }
    int getDuracao(){
        return this->duracao;
    }
    int getPrioridade(){
        return this->prioridade;
    };
    int getPid(){
        return this->pid;
    }
    // Sets
    void setDuracaoObservada(int d){
        this->duracao_observada = d;
    }
    void decDuracao(int d){
        if(d > this->duracao){
            this->duracao = 0;
        } else{
            this->duracao -= d;
        }
    }
    void changePrioridade(){
        if(this->prioridade == 0 || this->prioridade == 4){
            return;
        } else{
            if(this->decPrioridade){
                if(this->prioridade_atual < 4){
                    this->prioridade_atual += 1;
                } else{
                    this->decPrioridade = 0;
                    this->prioridade_atual -= 1;
                }
            } else{
                if(this->prioridade_atual > 1){
                    this->prioridade_atual -= 1;
                } else{
                    this->decPrioridade = 1;
                    this->prioridade_atual += 1;
                }
            }
        }
    }
};

class FilaRR{
private:
    vector<Processo> processos;
    int quantum = 1;    
    int time;
    vector<int> fila;
public:
    FilaRR(vector<Processo> processos){
        this->processos = processos;
        this->time = 0;
        int pa = 1000;
        int idx = 0;
        int processoAtualIdx = 0;
        // Como iterar sobre uma lista
        // Esse metodo so serve se a ordem do txt for modificada
        // Do jeito que esta e inutil
        vector<Processo> :: iterator it;
        for(it = processos.begin(); it != processos.end(); ++it){
            if(it->getChegada() < pa){
                pa = it->getChegada();
                processoAtualIdx = idx;
            }
            idx++;
        }
        if(pa == 0){
            fila.push_back(processoAtualIdx);
        }
        this->time = 0;
    }
// Gets

    int getTime(){
        return this->time;
    }
    void getPilha(){
        vector<int> :: iterator it;
        cout << "fila: ";
        for(it = this->fila.begin(); it != this->fila.end(); ++it){
            cout << this->processos[*it].getPid() << " ";
        }
        cout << endl;
    }
    int isEmpty(){
        return !(this->fila.size());
    }

    int tic(){
        vector<Processo> :: iterator it, it2;
        int idx=0;
        if(this->fila.empty()){
            int flag = 0;
            this->time += this->quantum;
            for(it = this->processos.begin(); it != this->processos.end(); ++it){
                if(it->getChegada() == this->time){
                    flag = 1;
                    break;
                }
                idx++;
            }
            if(flag){
                this->fila.push_back(idx);
            }
            return 0;
            
        } 
        int idxAtual = this->fila.front();
        this->fila.erase(this->fila.begin());
        //cout << "idxAtual: " << idxAtual << endl; 

        // it = processo[idxAtual]
        for(it = processos.begin(); it != processos.end(); ++it){
            if(idx == idxAtual){
                break;
            }
            idx++;
        }
        //cout << it->getDuracao() << endl;
        it->decDuracao(this->quantum);
        //cout << it->getDuracao() << endl;
        this->time += this->quantum;
        
        // Testa se entra algum processo NOVO no fim da fila
        // Se sim insere no fim da fila
        idx = 0;
        for(it2 = processos.begin(); it2 != processos.end(); ++it2){
            if(it2->getChegada() <= this->time && it2->getChegada() + this->quantum > this->getTime()){
                this->fila.push_back(idx);
            }
            idx++;
        }


        // Ve se o processo[idxAtual] ja terminou
        // Se sim armazena a duracao
        // Se nao insere ele novamente no final da fila
        if(it->getDuracao() == 0){
            it->setDuracaoObservada(this->time);
        } else{
            this->fila.push_back(idxAtual);
        }

        int d=0;
        for(it = processos.begin(); it != processos.end(); ++it){
            d+= it->getDuracao();
        }
        if(d == 0){
            return 0;
        }        
        return 1;
    } 
};

class CPU{
private:
    int nFilas;
    vector<FilaRR> filas; 
public:
    CPU(vector<Processo> processos){
        this->nFilas = 5;
        int idx = 0;
        vector<Processo> :: iterator it;
        vector<vector<Processo>> listP;
        //Vai mandando um processo pra cada FilaRR
        for(int i = 0; i < this->nFilas; i++){
            vector<Processo> p;
            listP.push_back(p);            
        }
        while(!processos.empty()){
            listP[processos.front().getPrioridade()].push_back(processos.front());
            processos.erase(processos.begin());
        }
        for (int i = 0; i < nFilas; i++){
            this->filas.push_back(FilaRR(listP[i]));
        }
    }
    // Errado, teria que executar somente uma das filas 
    // e olhar a prioridade
    int tic(){
        vector<FilaRR> :: iterator it;
        int filaAtual = -1;
        for(int i=0; i<this->filas.size();i++){
            if(!(this->filas[i].isEmpty())){
                filaAtual = i;
                break;
            }
        }
        if(filaAtual != -1){
            this->filas[filaAtual].tic();
        }
       
        int naoTerminou = 0;
        for(it = this->filas.begin(); it != this->filas.end(); ++it){
            naoTerminou += it->tic();
        }
        return naoTerminou;
    }
    void getInfo(){
        vector<FilaRR> :: iterator it;
        int idx = 0;
        for(it = this->filas.begin(); it != this->filas.end(); ++it){
            cout << "Fila(" << idx << "): ";
            it->getPilha();
            idx++;
        }
    }
  

};

int main(int argc, char *argv[]) {
    vector<Processo> processos;
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
        int pid = 0;
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

            processos.push_back(Processo(chegada, duracao, memoria, prioridade, pid));
            cout << "Chegada    : " << chegada << endl;
            cout << "Duração    : " << duracao << endl;
            cout << "Memória    : " << memoria << endl;
            cout << "Prioridade : " << prioridade << "\n" << endl;
            pid++;
        }
        processosFile.close();
        // cout << "tam: " << processos.size() << endl;
        // O que fazer:
        // primeiro, botar uma fila RR em cada cpu a medida que os processos vem chegando
        // quando todas CPUs estao cheias, adicionar os processos nas filasRR mais vazias
        // Temos que escolher um numero fixo de filasRR por CPU
        // Vamos enxendo as filas na medida que vao aparecendo novos processos
        // Ideia: criar uma classe CPU que gerencia a quantidade de filasRR na CPU
        // Criar uma classe escalonador que gerencia os processos para as CPUs 
        CPU c = CPU(processos);
        c.getInfo();
        while(c.tic()){
            c.getInfo();
        }
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

