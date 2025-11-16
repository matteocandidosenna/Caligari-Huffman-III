#ifndef CALIGARI_HPP
#define CALIGARI_HPP
#include <string>
#include <map>
#include <ctime>
#include <vector>
#include <algorithm>
using namespace std;

struct Registro{
    int id;
    int num_quarto;
    int idade;
    string nome;
    string diagnostico;
    string relatorio; //nome do arquivo do paciente -> será usado futuramente
    time_t data_admissao;
    time_t data_alta;
};

class Hospicio{
private: 
    map<int, bool> Ocupacao; //controla a ocupação de cada quarto
    map<int, Registro> Quartos; //depende diretamente do map Ocupacao
    int TotalQuartos; //atributo que armazena a quantidade de quartos do hospício
    int NumPacientesInternos; //controla a quantidade de internos que existem
    string ArquivoHistorico; //armazena os dados dos pacientes, data de admissão e alta
    string arquivo_dados; //para carregar a partir de um documento os dados do hospício previamente criado -> será futuramente usado

public:
    Hospicio(int TotalQuartos){
        this->TotalQuartos = TotalQuartos;
        this->NumPacientesInternos = 0;
        for(int i = 1; i <= this->TotalQuartos; i++) Ocupacao[i]=false; //incializando todas as quartos vazios
    }

    //métodos de acesso para buscas externas
    const map<int, Registro>& getQuartos() const {  //retorna o map dos quartos
        return Quartos; 
    }
    
    const map<int, bool>& getOcupacao() const {//retorna o map das ocupações dos quartos 
        return Ocupacao; 
    }
    
    int getTotalQuartos() const { //retorna a quantidade total de quartos
        return TotalQuartos; 
    }
    
    int getNumPacientes() const { //retorna a quantidade de internos do objeto
        return NumPacientesInternos; 
    }
    
    bool quartoOcupado(int numero) const { //retorna, dado um numero de quarto, se ele está ocupado ou não
        auto it = Ocupacao.find(numero);
        return it != Ocupacao.end() ? it->second : false;
    }

    //configuração de aqivos
    void setArquivoHistorico(const string& nome) { //configura o nome do arquivo do hopício
        ArquivoHistorico = nome;
    }
    
    void setArquivoDados(const string& nome) { //configura o nome do arquivo histórico do hospício
        arquivo_dados = nome;
    }

    //método para consltuar paciente no quarto
    Registro* consultarPaciente(int num_quarto) {
        auto it = Quartos.find(num_quarto);
        if(it != Quartos.end()) {
            return &it->second;
        }
        return nullptr;
    }

    //metodos para relatorio do paciente
    bool criarRelatorioPaciente(const Registro& reg);
    bool escreverNoRelatorio(const Registro& reg, const string& texto);
    string lerRelatorioPaciente(const Registro& reg) const;

    //Manipulação de arquivos
    bool salvarDadosTXT();
    bool carregarDadosTXT();
    void salvarHistoricoTXT(const Registro& reg, const string& evento);
    bool AdmitirPaciente(Registro& reg);
    bool DarAlta(Registro &reg);
};
const Registro* BuscaSequencialNome(const Hospicio &hosp, const string &nome);
bool BuscaBinariaPorQuarto(const Hospicio &hosp, int NumQuarto);
bool BuscaBinariaPorID(const Hospicio &hosp, int ID_paciente);

//Rabin Karp
vector<pair<string, int>> BuscaPadraoRabin(const Hospicio &hosp, const string &padrao);
bool RabinKarp(const string &texto, const string &padrao, vector<int> &ocorrencias);

#endif