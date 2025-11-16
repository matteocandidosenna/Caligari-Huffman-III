#include "caligari.hpp"
#include "huffman.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

//Rabin Karp
const int Primo = 97;
long calcularHash(const string &str, int inicio, int tam){
    long hash = 0;
    for(int i = inicio; i<inicio +tam; i++){
        hash = (hash * 256 + str[i] ) %Primo;
    }
    return hash;
}

bool RabinKarp(const string &texto, const string &padrao, vector<int> &ocorrencias){
    int n = texto.length();
    int m = padrao.length();
    if(m == 0 || n < m) return false;

    long hashPadrao = calcularHash(padrao, 0, m );
    long hashTexto = calcularHash(texto, 0, m );
    long potencia = 1;

    for(int i=0; i< m-1; i++) potencia = (potencia * 256) % Primo;
    bool encontrou = false;
    for(int i = 0; i <= n - m ; i++){
        if(hashPadrao == hashTexto){
            bool match = true;
            for(int j = 0; j < m; j++){
                if(texto[i+j]!=padrao[j]){
                    match = false;
                    break;
                }
            }
            if(match) ocorrencias.push_back(i);
            encontrou = true;
        }

        if(i < n-m){
            hashTexto = (256 * (hashTexto - texto[i] * potencia) + texto[i + m]) % Primo;
            if(hashTexto < 0) hashTexto += Primo;
        }
    }
    return encontrou;
}

vector<pair<string, int>> BuscaPadraoRabin(const Hospicio &hosp, const string &padrao){
    vector<pair<string, int>> resultados;
    const auto &quartos = hosp.getQuartos();

    for(const auto &par : quartos){
        const Registro &paciente= par.second;
        string conteudo = hosp.lerRelatorioPaciente(paciente);

        if(!conteudo.empty()){
            vector<int> ocorrencias;
            if(RabinKarp(conteudo, padrao, ocorrencias)){
                resultados.push_back({paciente.nome, static_cast<int>(ocorrencias.size())});
            }
        }
    }
    return resultados;
}

//relatorio
bool Hospicio::escreverNoRelatorio(const Registro &reg, const string &texto){
    string arquivoHuff = reg.relatorio+".huff";
    string existente;

    //lê e descomprime caso exista um relatorio antigo
    ifstream in(arquivoHuff, ios::binary);
    if(in.is_open()){
        stringstream buffer;
        buffer<<in.rdbuf();
        string conteudoComprimido = buffer.str();
        in.close();

        if(!conteudoComprimido.empty()) existente = HuffmanCompressor::decompress(conteudoComprimido);

        //acrescentando novo texto
        if(!existente.empty()) existente +="\n";
        existente += texto;

        //comprimindo dnv
        string comprimindoOut = HuffmanCompressor::compress(existente);
        ofstream out(arquivoHuff, ios::binary | ios::trunc);
        if(!out.is_open()) return false; //deu errado

        out<<comprimindoOut;
        out.close();

        return true;
    }
}   

string Hospicio::lerRelatorioPaciente(const Registro& reg) const {
    //tenta ler o compactado antes:
    string compressedFile = reg.relatorio + ".huff";
    ifstream testCompressed(compressedFile);
    
    if (testCompressed.is_open()) {
        testCompressed.close();
        //descompacta e retorna o conteúdo
        string tempFile = "temp_decompress.txt";
        if (HuffmanCompressor::decompressFile(compressedFile, tempFile)) {
            ifstream temp(tempFile);
            if (temp.is_open()) {
                stringstream buffer;
                buffer << temp.rdbuf();
                temp.close();
                remove(tempFile.c_str()); //limpa arquivo temporari
                return buffer.str();
            }
        }
    }
    
    // nao deu, lê o arquivo normal
    ifstream arquivo(reg.relatorio);
    if (!arquivo.is_open()) {
        return "Arquivo nao encontrado!";
    }
    
    stringstream buffer;
    buffer << arquivo.rdbuf();
    arquivo.close();
    return buffer.str();
}

bool Hospicio::criarRelatorioPaciente(const Registro& reg){
    ofstream arquivo(reg.relatorio);
    return arquivo.is_open();     
}

const Registro* BuscaSequencialNome(const Hospicio &hosp, const string &nome){
    const auto &quartos = hosp.getQuartos();
    for(const auto &pair: quartos){//iterando sobre o map dos quartos
        if(pair.second.nome == nome){//se o nome presente neste map for igual ao nome pesquisado
            return &pair.second;//retorna o ponteiro
        }
    }
    return nullptr;//nao encontrou, retorna ponteiro nulo
}

bool BuscaBinariaPorID(const Hospicio &hosp, int ID_paciente) {
    const auto &quartos = hosp.getQuartos();
    if(quartos.empty()) return false;

    // Extraindo e ordenando IDS
    std::vector<int> IdsOrdenados;
    for(const auto &pair : quartos) {
        IdsOrdenados.push_back(pair.second.id);
    }
    std::sort(IdsOrdenados.begin(), IdsOrdenados.end());

    int inicio = 0;
    int fim = IdsOrdenados.size() - 1;

    while(inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        if(IdsOrdenados[meio] == ID_paciente) return true;
        else if(IdsOrdenados[meio] < ID_paciente) inicio = meio + 1;
        else fim = meio - 1;
    }
    return false;
}

bool BuscaBinariaPorQuarto(const Hospicio &hosp, int NumQuarto){
    const auto &quartos = hosp.getQuartos();
    if(quartos.empty()) return false;

    vector<int> quartosOrdenados;
    for(const auto &pair : quartos) quartosOrdenados.push_back(pair.first);

    sort(quartosOrdenados.begin(), quartosOrdenados.end());

    int inicio = 0;
    int fim = quartosOrdenados.size()-1;

    while(inicio<=fim){
        int meio = (inicio+fim)/2;

        if(quartosOrdenados[meio]== NumQuarto) return true;

        else if(quartosOrdenados[meio] < NumQuarto) inicio = meio+1;

        else fim = meio - 1;
    }
    return false;
}

//Salvando em texto
bool Hospicio::salvarDadosTXT() {
    const string arquivo = "hospicio.txt";
    ofstream arq(arquivo);
    if(!arq.is_open()) return false;
    
    // Cabeçalho do arquivo
    arq << "=== HOSPICIO CALIGARI - DADOS ===" << endl;
    arq << "TotalQuartos: " << TotalQuartos << endl;
    arq << "PacientesInternos: " << NumPacientesInternos << endl;
    arq << endl;
    
    // Pacientes
    arq << "[PACIENTES]" << endl;
    for(const auto& pair : Quartos) {
        const Registro& reg = pair.second;
        arq << "Quarto:" << reg.num_quarto << endl;
        arq << "ID:" << reg.id << endl;
        arq << "Nome:" << reg.nome << endl;
        arq << "Diagnostico:" << reg.diagnostico << endl;
        arq << "Relatorio:" << reg.relatorio << endl;
        arq << "Idade:" << reg.idade << endl;
        arq << "Admissao:" << reg.data_admissao << endl;
        arq << "Alta:" << reg.data_alta << endl;
        arq << "---" << endl; // Separador
    }
    return true;
}

bool Hospicio::carregarDadosTXT() {
    const string arquivo = "hospicio.txt";
    ifstream arq(arquivo);
    if(!arq.is_open()) return false;
    
    // Limpa dados atuais
    Quartos.clear();
    for(auto& pair : Ocupacao) pair.second = false;
    NumPacientesInternos = 0;
    
    string linha;
    Registro reg;
    bool lendoPacientes = false;
    
    while(getline(arq, linha)) {
        if(linha.empty()) continue;
        
        if(linha == "[PACIENTES]") {//tem uma linha exatamete assim no cabecalho, por isso fiz a verificacao
            lendoPacientes = true;
            continue;
        }
        
        if(lendoPacientes) { //esta linha garante que o código só tentará ler dados de paciente se já tiver passado pelo cabeçalho do arquivo e entrado na seção de registros
            if(linha.find("Quarto:") != string::npos) { //npos -> quando a string nao é encontrada
                reg.num_quarto = stoi(linha.substr(7));
            }
            else if(linha.find("ID:") != string::npos) {
                reg.id = stoi(linha.substr(3));
            }
            else if(linha.find("Nome:") != string::npos) {
                reg.nome = linha.substr(5);
            }
            else if(linha.find("Diagnostico:") != string::npos) {
                reg.diagnostico = linha.substr(12);
            }
            else if(linha.find("Relatorio:") != string::npos) {
                reg.relatorio = linha.substr(10);
            }
            else if(linha.find("Idade:") != string::npos) {
                reg.idade = stoi(linha.substr(6));
            }
            else if(linha.find("Admissao:") != string::npos) {
                reg.data_admissao = stol(linha.substr(9));
            }
            else if(linha.find("Alta:") != string::npos) {
                reg.data_alta = stol(linha.substr(5));
            }
            else if(linha == "---") {
                //finalizou um paciente - salva no sistema
                if(reg.num_quarto > 0) {
                    Quartos[reg.num_quarto] = reg;
                    Ocupacao[reg.num_quarto] = true;
                    NumPacientesInternos++;
                }
                reg = Registro{}; // reseta para próximo
            }
        }
    }
    return true;
}

void Hospicio::salvarHistoricoTXT(const Registro& reg, const string& evento){
    ofstream arq("historico.txt", ios::app); //escrevendo no arquivo com append mode (nao apaga o conteudo existente, posicionando o cursor no ffim do arquivo)
    if(arq.is_open()) { //verifica se abiru com sucesso
        time_t agora = time(0);
        arq << "[" << ctime(&agora) << "] " << evento << " - "  //ctime converte o timestamp para legibilidade de humanos
             << reg.nome << " (Quarto " << reg.num_quarto << ")" << endl;
    }
}

bool Hospicio::AdmitirPaciente(Registro& reg){
    int numQuarto = reg.num_quarto;
    if(numQuarto < 1 || numQuarto > TotalQuartos) {
        // quarto inválido
        return false;
    }

    auto it = Ocupacao.find(numQuarto);
    if(it == Ocupacao.end()) {
        // caso Ocupacao não tenha sido inicializado corretamente, inicializar
        Ocupacao[numQuarto] = false;
    }

    if(!Ocupacao[numQuarto]){
        Ocupacao[numQuarto]=true;
        Registro novoReg = reg;
        novoReg.data_admissao = time(0);
        
        // garante o formato correto do Relatorio antes de salvar
        novoReg.relatorio = novoReg.nome + to_string(novoReg.id) + ".txt";

        Quartos[numQuarto]=novoReg;
        NumPacientesInternos++;
        
        // salva a admissao no historico
        salvarHistoricoTXT(novoReg, "ADMISSÃO");
        return true;
    }
    return false;
}

bool Hospicio::DarAlta(Registro &reg){
    int num_quarto = reg.num_quarto;
    auto it = Ocupacao.find(num_quarto);
    if(it != Ocupacao.end() && it->second){
        if(Quartos.find(num_quarto) != Quartos.end()) {
            Quartos[num_quarto].data_alta = time(0);
            
            // salva no historico antes de remover
            salvarHistoricoTXT(Quartos[num_quarto], "ALTA");
        }
        Ocupacao[num_quarto] = false;
        Quartos.erase(num_quarto);
        NumPacientesInternos--;
        return true;
    }
    return false;
}


