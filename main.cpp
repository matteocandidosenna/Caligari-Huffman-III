#include "caligari.hpp"
#include "huffman.hpp"
#include <iostream>

using namespace std;

void verQuartosOcupados(Hospicio &hosp){
    const auto& ocupacao = hosp.getOcupacao();
    int ocupados = 0;
    
    for(const auto& pair : ocupacao) {
        if(pair.second) {
            cout << pair.first << " ";
            ocupados++;
        }
    }
}

void limparBuffer(){
    cin.clear(); //para quando o usuário digita tipos errados
    cin.ignore(1000, '\n'); //do buffer, remove tudo até a quebra de linha
}

void pausar(){
    cout << "\nPressione Enter para continuar...";
    limparBuffer();
}

void mostrarMenu(){
    cout << "\n=== SISTEMA HOSPICIO CALIGARI ===" << endl;
    cout << "1. Admitir Paciente" << endl;
    cout << "2. Dar Alta" << endl;
    cout << "3. Buscar Paciente por Nome" << endl;
    cout << "4. Verificar Ocupacao do Quarto" << endl;
    cout << "5. Listar Todos os Pacientes" << endl;
    cout << "6. Consultar Paciente no Quarto" << endl;
    cout << "7. Ver Quartos Livres" << endl;
    cout << "8. Estatisticas" << endl;
    cout << "9. Escrever ou Ler Relatorio de um Paciente" <<endl;
    cout << "10. Buscar Padrao com Rabin Karp" <<endl;
    cout << "0. Sair" << endl;
    cout << "Escolha uma opcao: ";
}

void DepoimentosRelatorio(Hospicio &hosp){
    cout << "\n Depoimentos Delirantes "<<endl;
    cout <<" Escolha uma opcao"<<endl;
    cout<< "1. Ler Depoimento"<<endl;
    cout<< "2. Escreber Depoimento"<<endl;

    int opcao;
    cin>>opcao;
    limparBuffer();

    switch(opcao){
        case 1:{
        cout<<"Quartos com pacientes: ";
        verQuartosOcupados(hosp);
        cout<<endl;
        cout << "Numero do quarto: " <<endl;
        int quarto; 
        cin>>quarto;
        limparBuffer();

        Registro *paciente = hosp.consultarPaciente(quarto);
        if(paciente){
            string conteudo = hosp.lerRelatorioPaciente(*paciente);
            cout<< "Depoimentos de "<<paciente->nome<<": "<<endl;
            cout<<" ------------------------------------------ "<<endl; 
            if(conteudo.empty()) cout<<"Arquivo Vazio"<<endl;
            else cout<<conteudo<<endl;
        }
        else{
            cout<<"Paciente não encontrado!"<<endl;
        }
        break;
        }

        case 2:{
            cout << "Digite o numero do quarto"<<endl;
            cout <<"Quartos com pacientes: "<<endl;
            verQuartosOcupados(hosp);
            cout<<endl;
            int quarto;
            cin>>quarto;
            limparBuffer();

            Registro * paciente = hosp.consultarPaciente(quarto);
            if(paciente){
                cout<<"Escreva o depoimento do paciente: "<<endl;
                string texto;
                getline(cin, texto);

                if(hosp.escreverNoRelatorio(*paciente, texto)) cout << "Depoimento Registrado" <<endl;
                
                else cout <<"Erro ao registrar! "<<endl;
            }
            else cout<<"Paciente nao encontrado"<<endl;
            break;
        }
        default: cout<<"Opcao inválida"<<endl;
    }
}

void ExibirSalasValidas(Hospicio &hosp){
    cout<<"escolha entre 0 e "<<hosp.getTotalQuartos()<<endl;
    cout<<"As salas "<<endl;
    for(const auto it : hosp.getOcupacao()){
        if(it.second == false) cout<<" "<<it.first;
    }
    cout<<endl<<" estao livres. Digite onde quer coloca-lo"<<endl;
}

void BuscarRabinMenu(Hospicio &hosp){
    limparBuffer();
    cout<<"\n Busca Rabin Karp" <<endl;
    cout<<"Digite o padrao que deseja encontrar nos depoimentos"<<endl;
    string padrao;
    getline(cin, padrao);

    if(padrao.empty()){
        cout<<"Padrao invalido"<<endl;
        return;
    }

    vector<pair<string, int>> resultados = BuscaPadraoRabin(hosp, padrao);
    cout<< "Resultados da busca" <<endl;
    cout<<"Padrao: "<<padrao<<endl;
    cout<<"---------------------------------------------"<<endl;

    if(resultados.empty()) cout<< "Nenhum padrao fora encontrado"<<endl;
    else{
        cout<<"Paciente com o padrao: "<<endl;
        for(const auto &resultado : resultados){
            if(resultado.second>0) cout<<resultado.first<<" - "<<resultado.second<<" ocorrencias"<<endl;
        }
        
    }
}

Registro lerDadosPaciente(Hospicio &hosp){
    Registro reg;

    int input_id;
    int input_quarto;
    
    cout << "\n--- Dados do Paciente ---" << endl;
    
    // verificacao de ID duplicado
    do {
        cout << "ID (para checagem de duplicidade): ";
        // a leitura pode falhar
        if (!(cin >> input_id)) { 
            cout << "ERRO: Entrada invalida. Por favor, digite um numero inteiro para o ID." << endl;
            limparBuffer();
            continue; // recomeça o loop
        }
        
        if (BuscaBinariaPorID(hosp, input_id)) {
            cout << "ERRO: ID " << input_id << " ja esta em uso. Digite outro ID." << endl;
        } else {
            reg.id = input_id;
            break;
        }
    } while(true);
    
    // limpeza da quabra de linha de quando lemos o ID
    limparBuffer();
    
    cout << "Nome: ";
    getline(cin, reg.nome); 
    
    cout << "Diagnostico: ";
    getline(cin, reg.diagnostico);
    
    // arquivo relatorio
    reg.relatorio = reg.nome + to_string(reg.id) + ".txt";
    
    do {
        cout << "Numero do quarto: ";
        ExibirSalasValidas(hosp); // Mostra as opções
        
        if (!(cin >> input_quarto)) {
            cout << "ERRO: Entrada invalida. Por favor, digite um numero de quarto valido." << endl;
            limparBuffer();
            
            continue;
        }
        // hceca se o quarto está no intervalo válido E se está livre
        if (input_quarto > 0 && input_quarto <= hosp.getTotalQuartos() && !hosp.quartoOcupado(input_quarto)) {
            reg.num_quarto = input_quarto;
            break;
        } else {
            cout << "ERRO: O quarto " << input_quarto << " e invalido ou ja esta ocupado!" << endl;
        }
    } while(true);

    cout << "Idade: ";
    cin >> reg.idade; 
    
    return reg;
}

void admitirPaciente(Hospicio &hosp){
    cout << "\n=== ADMITIR PACIENTE ===" << endl;
    Registro reg = lerDadosPaciente(hosp);
    
    if(hosp.AdmitirPaciente(reg)) {
        cout << "Paciente admitido com sucesso no quarto " << reg.num_quarto << "!" << endl;
    } else {
        cout << "Erro: Quarto " << reg.num_quarto << " já ocupado ou invalido!" << endl;
    }
}

void darAlta(Hospicio &hosp){
    cout << "\n=== DAR ALTA ===" << endl;
    int num_quarto;
    cout<<"Quartos atualmente em uso: ";
    verQuartosOcupados(hosp);
    cout <<endl<< "Numero do quarto para alta: ";
    cin >> num_quarto;
    
    // Cria registro só com o Numero do quarto
    Registro reg;
    reg.num_quarto = num_quarto;
    
    if(hosp.DarAlta(reg)) {
        cout << "Alta concedida para o quarto " << num_quarto << "!" << endl;
    } else {
        cout << "Erro: Quarto " << num_quarto << " nao encontrado ou ja vazio!" << endl;
    }
}

void buscarPorNome(Hospicio &hosp){
    cout << "\n=== BUSCAR POR NOME (busca sequencial) ===" << endl;
    string nome;
    cout << "Nome do paciente: ";
    limparBuffer();
    getline(cin, nome);
    
    const Registro* encontrado = BuscaSequencialNome(hosp, nome);
    
    if(encontrado) {
        cout << "\n PACIENTE ENCONTRADO:" << endl;
        cout << "Nome: " << encontrado->nome << endl;
        cout << "Quarto: " << encontrado->num_quarto << endl;
        cout << "diagnostico: " << encontrado->diagnostico << endl;
        cout << "Idade: " << encontrado->idade << " anos" << endl;
    } else {
        cout << "Paciente nao encontrado!" << endl;
    }
}

void verificarQuarto(Hospicio &hosp){
    cout << "\n=== VERIFICAR QUARTO ===" << endl;
    int num_quarto;
    cout << "Numero do quarto: ";
    cin >> num_quarto;
    
    if(BuscaBinariaPorQuarto(hosp, num_quarto)) {
        cout << "Quarto " << num_quarto << " esta OCUPADO" << endl;
        
        // Mostra informações do paciente
        Registro* paciente = hosp.consultarPaciente(num_quarto);
        if(paciente) {
            cout << "Paciente: " << paciente->nome << endl;
        }
    } else {
        cout << "Quarto " << num_quarto << " esta LIVRE" << endl;
    }
}

void listarPacientes(Hospicio &hosp){
    cout << "\n=== LISTA DE PACIENTES ===" << endl;
    const auto& quartos = hosp.getQuartos();
    
    if(quartos.empty()) {
        cout << "Nenhum paciente internado." << endl;
        return;
    }
    
    for(const auto& pair : quartos) {
        const Registro& reg = pair.second;
        cout << "Quarto " << reg.num_quarto << ": " << reg.nome 
             << " - " << reg.diagnostico << " (" << reg.idade << " anos)" << endl;
    }
}

void consultarPacienteQuarto(Hospicio &hosp){
    cout << "\n=== CONSULTAR PACIENTE NO QUARTO ===" << endl;
    int num_quarto;
    cout << "Numero do quarto: ";
    cin >> num_quarto;
    
    Registro* paciente = hosp.consultarPaciente(num_quarto);
    
    
    if(paciente) {
        cout << "\nDADOS DO PACIENTE:" << endl;
        cout << "Nome: " << paciente->nome << endl;
        cout << "ID: " << paciente->id << endl;
        cout << "diagnostico: " << paciente->diagnostico << endl;
        cout << "Idade: " << paciente->idade << " anos" << endl;
        cout << "Arquivo: " << paciente->relatorio << endl;
        cout << "Quarto: " << paciente->num_quarto << endl;
    } else {
        cout << "Quarto vazio ou nao existe!" << endl;
    }
}

void verQuartosLivres(Hospicio &hosp){
    cout << "\n=== QUARTOS LIVRES ===" << endl;
    const auto& ocupacao = hosp.getOcupacao();
    int livres = 0;
    
    cout << "Quartos livres: ";
    for(const auto& pair : ocupacao) {
        if(!pair.second) {
            cout << pair.first << " ";
            livres++;
        }
    }
    cout << "\nTotal de quartos livres: " << livres << "/" << hosp.getTotalQuartos() << endl;
}

void mostrarEstatisticas(Hospicio &hosp){
    cout << "\n=== ESTATISTICAS ===" << endl;
    cout << "Total de quartos: " << hosp.getTotalQuartos() << endl;
    cout << "Pacientes internados: " << hosp.getNumPacientes() << endl;
    cout << "Quartos ocupados: " << hosp.getNumPacientes() << endl;
    cout << "Quartos livres: " << (hosp.getTotalQuartos() - hosp.getNumPacientes()) << endl;
    cout << "Taxa de ocupacao: " << 
        (hosp.getNumPacientes() * 100.0 / hosp.getTotalQuartos()) << "%" << endl;
}

int main(){
    Hospicio hospicio(50); // Hospício com 50 quartos
    int opcao;

    if(hospicio.carregarDadosTXT()) {
        cout << "Dados carregados de hospicio.txt" << endl;
        cout << hospicio.getNumPacientes() << " pacientes encontrados" << endl;
    }
    else cout<<"Inciando com hospício vazio"<<endl;
    cout << "Bem vindo ao hospicio do DR. Caligari" << endl;
    
    do {
        mostrarMenu();
        cin >> opcao;
        
        switch(opcao) {
            case 1: admitirPaciente(hospicio); hospicio.salvarDadosTXT(); break;
            case 2: darAlta(hospicio); hospicio.salvarDadosTXT();break;
            case 3: buscarPorNome(hospicio); break;
            case 4: verificarQuarto(hospicio); break;
            case 5: listarPacientes(hospicio); break;
            case 6: consultarPacienteQuarto(hospicio); break;
            case 7: verQuartosLivres(hospicio); break;
            case 8: mostrarEstatisticas(hospicio); break;
            case 9: DepoimentosRelatorio(hospicio); break;
            case 10: BuscarRabinMenu(hospicio); break;

            case 0:
            cout << "Salvando dados..." << endl;
                hospicio.salvarDadosTXT();
                cout << "Saindo do sistema..." << endl;
                break;
            default:
                cout << "Opção inválida!" << endl;
        }
        
        if(opcao != 0) {
            pausar();
        }
        
    } while(opcao != 0);
    
    return 0;
}





