#include "huffman.hpp"
#include <fstream>
#include <sstream>
#include <bitset>
#include <iostream>

using namespace std;

string HuffmanCompressor::compress(const string& text) {
    if (text.empty()) return "";
    
    // Calcula frequência
    map<char, int> frequency = calculateFrequency(text);
    
    // Constrói árvore de Huffman
    HuffmanNode* root = buildHuffmanTree(frequency);
    
    // Gera códigos
    map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);
    
    // Serializa a árvore
    string treeData = serializeTree(root);
    
    // Codifica o texto
    string encodedText;
    for (char ch : text) {
        encodedText += huffmanCodes[ch];
    }
    
    // Converte string de bits para bytes
    string compressedData = treeData + "\n"; // Separador entre árvore e dados
    
    // Adiciona o tamanho do texto original (para descompressão)
    compressedData += to_string(text.length()) + "\n";
    
    // Converte bits para bytes
    string byteString;
    for (size_t i = 0; i < encodedText.length(); i += 8) {
        bitset<8> bits;
        for (size_t j = 0; j < 8 && i + j < encodedText.length(); j++) {
            bits[7 - j] = (encodedText[i + j] == '1');
        }
        byteString += char(bits.to_ulong());
    }
    compressedData += byteString;
    
    clearTree(root);
    return compressedData;
}

string HuffmanCompressor::decompress(const string& compressedText) {
    if (compressedText.empty()) return "";
    
    size_t pos = 0;
    
    // Desserializa a árvore
    HuffmanNode* root = deserializeTree(compressedText, pos);
    if (!root) return "";
    
    // Pula o separador
    pos++;
    
    // Lê o tamanho do texto original
    string lengthStr;
    while (pos < compressedText.length() && compressedText[pos] != '\n') {
        lengthStr += compressedText[pos++];
    }
    pos++; // Pula o \n
    
    int originalLength = stoi(lengthStr);
    
    // Converte bytes para bits
    string bitString;
    for (size_t i = pos; i < compressedText.length(); i++) {
        bitset<8> bits(compressedText[i]);
        bitString += bits.to_string();
    }
    
    // Decodifica o texto
    string decodedText;
    HuffmanNode* current = root;
    
    for (char bit : bitString) {
        if (decodedText.length() == originalLength) break;
        
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        
        if (!current->left && !current->right) {
            decodedText += current->character;
            current = root;
        }
    }
    
    clearTree(root);
    return decodedText;
}

bool HuffmanCompressor::compressFile(const string& inputFile, const string& outputFile) {
    ifstream input(inputFile, ios::binary);
    if (!input.is_open()) return false;
    
    stringstream buffer;
    buffer << input.rdbuf();
    string text = buffer.str();
    input.close();
    
    string compressed = compress(text);
    
    ofstream output(outputFile, ios::binary);
    if (!output.is_open()) return false;
    
    output << compressed;
    output.close();
    
    return true;
}

bool HuffmanCompressor::decompressFile(const string& inputFile, const string& outputFile) {
    ifstream input(inputFile, ios::binary);
    if (!input.is_open()) return false;
    
    stringstream buffer;
    buffer << input.rdbuf();
    string compressed = buffer.str();
    input.close();
    
    string decompressed = decompress(compressed);
    
    ofstream output(outputFile, ios::binary);
    if (!output.is_open()) return false;
    
    output << decompressed;
    output.close();
    
    return true;
}

map<char, int> HuffmanCompressor::calculateFrequency(const string& text) {
    map<char, int> frequency;
    for (char ch : text) {
        frequency[ch]++;
    }
    return frequency;
}

HuffmanNode* HuffmanCompressor::buildHuffmanTree(const map<char, int>& frequency) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> pq;
    
    for (const auto& pair : frequency) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }
    
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        
        HuffmanNode* parent = new HuffmanNode(left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        
        pq.push(parent);
    }
    
    return pq.top();
}

void HuffmanCompressor::generateCodes(HuffmanNode* root, const string& code, map<char, string>& huffmanCodes) {
    if (!root) return;
    
    if (!root->left && !root->right) {
        huffmanCodes[root->character] = code;
        return;
    }
    
    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

string HuffmanCompressor::serializeTree(HuffmanNode* root) {
    if (!root) return "";
    
    if (!root->left && !root->right) {
        return "1" + string(1, root->character);
    }
    
    return "0" + serializeTree(root->left) + serializeTree(root->right);
}

HuffmanNode* HuffmanCompressor::deserializeTree(const string& data, size_t& pos) {
    if (pos >= data.length()) return nullptr;
    
    if (data[pos] == '1') {
        pos++;
        if (pos >= data.length()) return nullptr;
        return new HuffmanNode(data[pos++], 0);
    }
    
    pos++; // Pula o '0'
    HuffmanNode* node = new HuffmanNode(0);
    node->left = deserializeTree(data, pos);
    node->right = deserializeTree(data, pos);
    
    return node;
}

void HuffmanCompressor::clearTree(HuffmanNode* root) {
    if (!root) return;
    
    clearTree(root->left);
    clearTree(root->right);
    delete root;
}