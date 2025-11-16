# Caligari-Huffman-III
Terceira versão do software designado à disciplina Estrutura de Dados II

ARQUITETURA GERAL DO PROGRAMA

  caligari.hpp: 
    Armazena a estrutura Registro, que representa os atributos do paciente do hospício;
    Armazena a classe hospício, que dita como deve ser a estrutura do hospício
      A classe hospício:
        Possui arquivos histórico;
        Tem um construtor que é inicializado com a quantidade de quartos que o hospício deve possuir;
        Possui métodos de acesso, que: 
          Retornam o total de quartos ocupados e o numero de internos,
          Retornam maps que mapeiam ocupação e registro;
        E operações de:
          Consulta de dados do paciente a partir do número do quarto;
          Criação, escrita e leitura de relatórios de pacintes;
          Admissão e alta de pacientes;
    Funções de busca linear e binária;
    Possui a assinatura das funções do algoritmo Rabin Karp.

  Análise Big(O) para as funções de busca binária e sequencial
      No programa, os dados dos pacientes e dos quartos estao armazenados em maps. Por isso foi necessário, antes de efetuar as buscas, passar por um loop 
        de custo O(n) os n quartos ocupados do hospício para um vetor. A partir deste vetor, é possível efetuar as buscas binária e sequencial.
        Complexidade da busca binária: O(log n) -> o vetor é ordenado com sort(), da biblioteca algorithm
        Complexidade da busca binária: O(n)
    
  caligari.cpp: Implementa as funções assinadas no arquivo caligari.hpp

  huffman.hpp: 
    Abriga a estrutura para os nós da árvore de Huffman;
    Um comparador para a priority queue;
    A classe Huffman compressor, com a assinatura para as funções do algoritmo de huffman.
    
  huffman.cpp: implementa as funções da classe HuffmanCompressor

  main.cpp: execução das funçoes, leituras e menu


COMO CADA ALGORITMO FOI IMPLEMENTADO E INTEGRADO:
  Busca Sequencial: busca pelo nome do paciente nos registros, verificando a existência daquele nome.
  A implementação: recebe como parâmetro um objeto da classe hospício e uma string contendo o nome. Em seguida, usa o método getquartos
  para obter as informações dos quartos, que é um map composto de ids(numero do quarto) e registros(struct que armazena dados de pacientes). Itera sobre o map
  e retorna se o paciente existe nos registros.

  Busca Binária por id: foram implementadas duas busca binárias: por id e por quarto - ambas funcionam de maneira semelhante. A função responsável por buscar ids 
  recebe
  como parâmetro os mesmos atributos que a busca sequencial. Em seguida, extrai os ids do map, armazenando-os num vetor. O map já entrega os ids ordenados ao vetor,
  por fim, aplica-se a algoritmo de fato: dividir para consquistar;
  Busca binária por quarto:Parecida com a busca por ids, só que faz isso com números de quartos;

  Rabin Karp: A busca Rabin Karp atua sobre os relatórios dos internos. No contexto do filme "O ganinete do dr. Caligari", há investigações sobre documentos - por isso, rabin Karp foi integrado ao sistema. A busca com rabin Karp retorna quantas evzes o parão de texto pesquisado aparecce no reltório de cada paciente.
  Esta função retorna um vetor de pares(string, int), com o nome do paciente e as ocorrências do padrão pesquisado em seu relatório.

  Huffman: A implementação mais complexa até agora, pois foi necessário alterar as funções de salvamento, leitura e carregamento de dados.
  para huffman, foram criados um .hpp e um .cpp a parte. 
  O algoritmo de huffman foi armazenado numa classe, na qual abriga suas funções essenciais.
  Também, no header do algoritmo de Hufffman, está abrigada a estrutura do nó da árvore de Huffman e o comparados para a priority queue.

  Análise da complexidade dos algoritmos implementados;
  Busca binária: O(log n);
  Busca sequencial: O(n);
  Rabin Karp: O(n*m), onde n é o comprimento do texto e m é o comprimento do padrão;
  Compressão de Huffman: O(n log n) onde n é o número de caracteres contidos no texto a ser comprimido.


  Extra:
  codigo para executar o programa no terminal: 
  
    g++ .\caligari.cpp .\huffman.cpp .\main.cpp -o hospicio
    .\hospicio.exe
  
  Repositório Github: https://github.com/matteocandidosenna/Caligari-Huffman-III
  
          
          
