#include <stdlib.h>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

class Processo{
private:
    int chegada, duracao, memoria, prioridade, lancamento, duracao_observada, duracao_projetada, prioridade_atual, pid, decPrioridade, timeExe;

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
        this->timeExe = 0;
    }
    // Gets
    int getChegada(){
        return this->chegada;
    }
    int getDuracao(){
        return this->duracao;
    }
    int getPrioridade_atual(){
        return this->prioridade_atual;
    };
    int getPid(){
        return this->pid;
    }
    int getTimeExe(){
        return this->timeExe;
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
    void incTimeExe(){
        this->timeExe += 1;
        if(this->timeExe % 10 == 0){
            cout << "ChagePrioridade" << endl;
            this->changePrioridade();
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
    int getSizeProcess(){
        return this->processos.size();
    }

    int getTime(){
        return this->time;
    }
    void getPilha(){
        vector<int> :: iterator it;
        cout << "fila: ";
        for(int i = 0; i < this->fila.size(); i++){
            cout << this->processos[this->fila[i]].getPid() << " ";
            // cout << this->fila[i] << " ";
        }
        // for(it = this->fila.begin(); it != this->fila.end(); ++it){
        //     cout << this->processos[*it].getPid() << " ";
        // }
        cout << endl;
    }
    int getProcessoTimeExe(int n){
        return this->processos[n].getTimeExe();
    }
    Processo getUltimoProcesso(){
        return this->processos[this->fila.back()];
    }
    int getNumProcessos(){
        return this->processos.size();
    }
    int isEmpty(){
        return !(this->fila.size());
    }
    int getTamFila(){
        return this->fila.size();
    }
    void getProcessInfo(){
        vector<Processo> :: iterator it;
        cout << "Info: ";
        for(it = processos.begin(); it != processos.end(); ++it){
            cout << it->getPid() << " ";
        }
        cout << endl;
    }
    Processo removeUltimoProcesso(){
        this->getPilha();
        this->getProcessInfo();
        cout << "Remocao de processo: " << endl;
        cout << "Size fila: " << this->fila.size() << endl;
        cout << "Size processos: " << this->processos.size() << endl;
        Processo p = this->processos[this->fila.back()];
        for(int i = 0; i < this->fila.size(); i++){
            if(this->fila[i] > this->fila.back())
                this->fila[i] -= 1;
        }

        //Depois dessa linha a pilha fica com valores errados
        this->processos.erase(this->processos.begin() + this->fila.back());
        this->fila.pop_back();
        this->getProcessInfo();
        this->getPilha();
        cout << "Size fila:" << this->fila.size() << endl;
        cout << "Size processos: " << this->processos.size() << endl;
        return p;
    }
    void atualizaFila(){
        Processo p = this->processos.back();
        int idx = -1;
        for(int i = 0; i< this->processos.size(); i++){
            if(p.getPid() == this->processos[i].getPid()){
                idx = i;
            }
        }
        // Bota o ultimo processo no final da fila
        if(idx != -1)
            this->fila.push_back(idx);
        for(int i = 0; i < this->processos.size(); i++){
            cout << "PID: " << this->processos[i].getPid() << endl;
        }


    }
    void pushProcesso(Processo p){

        this->processos.push_back(p);
        // for(int i = 0; i < this->processos.size(); i++){
        //         cout << "PID: " << this->processos[i].getPid() << endl;
        // }

    }
    void incTime(){
        vector<Processo> :: iterator it;
        int idx=0;
        this->time += this->quantum;
        if(this->fila.empty()){
            int flag = 0;
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
        }
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

        this->processos[idxAtual].decDuracao(this->quantum);
        this->processos[idxAtual].incTimeExe();


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
        cout << "Duracao: " << this->processos[idxAtual].getDuracao() << endl;
        if(this->processos[idxAtual].getDuracao() == 0){
            this->processos[idxAtual].setDuracaoObservada(this->time);
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
            listP[processos.front().getPrioridade_atual()].push_back(processos.front());
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
        int naoTerminou;
        for(int i=0; i<this->filas.size();i++){
            if(!(this->filas[i].isEmpty())){
                filaAtual = i;
                break;
            }
        }
        cout << "filaAtual: " << filaAtual << endl;
        if(filaAtual != -1){
            // Seg Fault aqui
            this->filas[filaAtual].tic();

            for(int i=0; i<this->filas.size();i++){
                if(filaAtual != i){
                    this->filas[i].incTime();
                }
            }

            // cout << "Prioridade Processo: " << this->filas[filaAtual].getUltimoProcesso().getPrioridade() << endl;
            // cout << "Prioridade da fila Atual: " << filaAtual << endl;
            //cout << "this->filas[filaAtual].getUltimoProcesso().getPrioridade_atual(): " << this->filas[filaAtual].getUltimoProcesso().getPrioridade_atual() << endl;
            if(this->filas[filaAtual].getUltimoProcesso().getPrioridade_atual() != filaAtual){
                if(this->filas[filaAtual].getTamFila()){
                    Processo p = this->filas[filaAtual].removeUltimoProcesso();
                    int priorP = p.getPrioridade_atual();
                    //cout << "Fila[" << priorP << "] :" << this->filas[priorP].getNumProcessos() << endl;
                    // Insere corretamente, atualizar as filas
                    //this->filas[priorP]
                    this->filas[priorP].pushProcesso(p);
                    //cout << "Fila[" << priorP << "] :" << this->filas[priorP].getNumProcessos() << endl;
                    this->filas[priorP].atualizaFila();
                } else{
                    return 0;
                }
            }
            return 1;
        } else{

            return 0;
        }
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

    int getSize(int i){
        return this->filas[i].getSizeProcess();
    }

    int getTimeProcess(int i, int j){
        return this->filas[i].getProcessoTimeExe(j);
    }
};

class Escalonador{
private:
    int nCPUs;
    vector<CPU> CPUs;
public:
    Escalonador(vector<Processo> processos, int numCPU){
        this->nCPUs = numCPU;
        vector<vector<Processo>> listP;
        vector<vector<vector<Processo>>> cpus;

        //Vai mandando um processo pra cada FilaRR
        for(int i = 0; i < 5; i++){
            vector<Processo> p;
            listP.push_back(p);
        }

        //criando filas para as cpus
        for(int i = 0; i < this->nCPUs; i++) {
            cpus.push_back(vector<vector<Processo> >());
            for (int j = 0; j < 5; ++j) {
                cpus[i].push_back(vector<Processo>());
            }
        }

        while(!processos.empty()){
            int ultimo = 1000;
            int index = -1;
            for (int i = 0; i < this->nCPUs; ++i) {
                for (int j = 0; j < 5; ++j) {
                    if (processos.front().getPrioridade_atual() == j){
                        if (cpus[i][j].size() < ultimo){
//                            cout << cpus[i][j].size() << endl;
                            ultimo = cpus[i][j].size();
                            index = i;
                            break;
                        }
                    }
                }
            }
            cpus[index][processos.front().getPrioridade_atual()].push_back(processos.front());
            processos.erase(processos.begin());
        }

        for (int m = 0; m < this->nCPUs; ++m) {
            vector<Processo> k;
            for (int l = 0; l < 5; ++l) {
                int size = cpus[m][l].size();
                for (int i = 0; i < size; ++i) {
                    k.push_back(cpus[m][l].front());
                    cpus[m][l].erase(cpus[m][l].begin());
                }
            }

            for (int j = 0; j < k.size(); ++j) {
                cout << k[j].getPrioridade_atual() << endl;
            }
            cout << "\n";
            CPU cp(k);
            this->CPUs.push_back(cp);
        }
    }

    void processoCPU(){
        for (int i = 0; i < this->CPUs.size(); ++i) {
            for (int j = 0; j < 5; ++j) {
                for (int k = 0; k < this->CPUs[i].getSize(j); ++k) {
                    cout << this->CPUs[i].getTimeProcess(j, k) << endl;
                }
            }
        }
    }

    int ticCPU(int index){
        return this->CPUs[index].tic();
    }

    void getInfoCPU(int index){
        this->CPUs[index].getInfo();
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
            getline (processosFile,line, '\n');
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

        Escalonador e(processos, numCPU);
        for (int i = 0; i < numCPU; ++i) {
            cout << "CPU[" << i << "]" << endl;
            e.getInfoCPU(i);
            cout << "**********************************************" << endl;
            while(e.ticCPU(i)){
                cout << "CPU[" << i << "]" << endl;
                e.getInfoCPU(i);
                cout << "**********************************************" << endl;
            }
        }
        e.processoCPU();
    }
    else cout << "Unable to open file" << endl;
}
// teste se o processo ta na fila na hora do tic