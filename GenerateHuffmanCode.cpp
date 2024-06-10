/*
This program can be used to get the encoded Huffman string which is ued in the Load.exe binary. Replace the PowerShell script with the script given or your custom PS script. 
*/

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <bitset>

// Node structure for Huffman Tree
struct HuffmanNode {
    char data;
    unsigned frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, unsigned frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}

    ~HuffmanNode() {
        delete left;
        delete right;
    }
};

struct Compare {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->frequency > right->frequency;
    }
};

HuffmanNode* buildHuffmanTree(const std::unordered_map<char, unsigned>& freqMap) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;

    for (const auto& pair : freqMap) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() != 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        HuffmanNode* parent = new HuffmanNode('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    return pq.top();
}

void buildCodes(HuffmanNode* root, const std::string& code, std::unordered_map<char, std::string>& codes) {
    if (!root)
        return;
    if (root->data != '\0')
        codes[root->data] = code;
    buildCodes(root->left, code + "0", codes);
    buildCodes(root->right, code + "1", codes);
}

std::string encode(const std::string& data, const std::unordered_map<char, std::string>& codes) {
    std::string encoded;
    for (char c : data) {
        encoded += codes.at(c);
    }
    return encoded;
}

int main() {
    std::string data = "<PS_script>";
    std::unordered_map<char, unsigned> freqMap;
    for (char c : data) {
        freqMap[c]++;
    }
  
    std::cout << "Character Frequencies:\n";
    for (const auto& pair : freqMap) {
        std::cout << "'" << pair.first << "': " << pair.second << std::endl;
    }

    HuffmanNode* root = buildHuffmanTree(freqMap);

    std::unordered_map<char, std::string> codes;
    buildCodes(root, "", codes);

    std::string encoded = encode(data, codes);

    std::cout << "Original: " << data << std::endl;
    std::cout << "Encoded: " << encoded << std::endl;

  delete root;
    
    return 0;
}
