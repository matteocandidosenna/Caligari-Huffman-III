#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <string>
#include <vector>
#include <map>
#include <queue>

// Estrutura para os nós da árvore de Huffman
struct HuffmanNode {
    char character;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
    
    HuffmanNode(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
    HuffmanNode(int freq) : character('\0'), frequency(freq), left(nullptr), right(nullptr) {}
};

// Comparador para a priority_queue
struct CompareNodes {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

class HuffmanCompressor {
public:
    // Comprime uma string usando Huffman
    static std::string compress(const std::string& text);
    
    // Descomprime uma string usando Huffman
    static std::string decompress(const std::string& compressedText);
    
    // Comprime um arquivo
    static bool compressFile(const std::string& inputFile, const std::string& outputFile);
    
    // Descomprime um arquivo
    static bool decompressFile(const std::string& inputFile, const std::string& outputFile);

private:
    // Calcula a frequência dos caracteres
    static std::map<char, int> calculateFrequency(const std::string& text);
    
    // Constrói a árvore de Huffman
    static HuffmanNode* buildHuffmanTree(const std::map<char, int>& frequency);
    
    // Gera os códigos de Huffman
    static void generateCodes(HuffmanNode* root, const std::string& code, std::map<char, std::string>& huffmanCodes);
    
    // Serializa a árvore de Huffman
    static std::string serializeTree(HuffmanNode* root);
    
    // Desserializa a árvore de Huffman
    static HuffmanNode* deserializeTree(const std::string& data, size_t& pos);
    
    // Limpa a árvore de Huffman
    static void clearTree(HuffmanNode* root);
};

#endif