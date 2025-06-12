#include "WordFreq.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

struct AVLNode {
    WordFreq data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const WordFreq& wf) : data(wf), left(nullptr), right(nullptr), height(1) {}
};

// Bir d���m�n y�ksekli�ini d�ner
int height(AVLNode* node) {
    if (node == nullptr) return 0; // Bo� d���mse y�kseklik 0
    return node->height;
}

// Bir d���m�n denge fakt�r�n� hesaplar
int getBalance(AVLNode* node) {
    if (node == nullptr) return 0; // Bo� d���mde denge fakt�r� 0
    return height(node->left) - height(node->right);
}

// Bir d���m�n y�ksekli�ini g�nceller
void updateHeight(AVLNode* node) {
    if (node) node->height = 1 + max(height(node->left), height(node->right));
}

// Sa�a D�nd�rme (Right Rotate)
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left; // Y'nin sol alt d���m� X olur
    AVLNode* T2 = x->right; // X'in sa� alt a�ac�n� tut

    // Sa�a d�nd�rme i�lemi
    x->right = y;
    y->left = T2;

    // Y�kseklikleri g�ncelle
    updateHeight(y);
    updateHeight(x);

    return x; // Yeni k�k� d�nd�r
}

// Sola D�nd�rme (Left Rotate)
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right; // X'in sa� alt d���m� Y olur
    AVLNode* T2 = y->left; // Y'nin sol alt a�ac�n� tut

    // Sola d�nd�rme i�lemi
    y->left = x;
    x->right = T2;

    // Y�kseklikleri g�ncelle
    updateHeight(x);
    updateHeight(y);

    return y; // Yeni k�k� d�nd�r
}

// AVL A�ac�na bir WordFreq ekler
AVLNode* insert(AVLNode* node, const WordFreq& wf) {
    if (node == nullptr) return new AVLNode(wf); // Bo� d���mse yeni d���m olu�tur

    // Kelime s�ras�na g�re ekleme yap�l�r
    if (wf.word < node->data.word) {
        node->left = insert(node->left, wf); // Sol tarafa ekle
    }
    else if (wf.word > node->data.word) {
        node->right = insert(node->right, wf); // Sa� tarafa ekle
    }
    else {
        // E�er kelime zaten varsa, frekans�n� g�ncelle
        node->data.freq = wf.freq;
        return node;
    }

    // Y�ksekli�i g�ncelle
    updateHeight(node);

    // Denge fakt�r�n� kontrol et ve AVL dengesini sa�la
    int balance = getBalance(node);

    // Sol-Sol durumu
    if (balance > 1 && wf.word < node->left->data.word) return rightRotate(node);

    // Sa�-Sa� durumu
    if (balance < -1 && wf.word > node->right->data.word) return leftRotate(node);

    // Sol-Sa� durumu
    if (balance > 1 && wf.word > node->left->data.word) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Sa�-Sol durumu
    if (balance < -1 && wf.word < node->right->data.word) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node; // Dengelenmi� d���m� d�nd�r
}

// AVL a�ac�n� s�ral� bir �ekilde dola��r
void inorder(AVLNode* root, vector<WordFreq>& words) {
    if (root == nullptr) return; // Bo� d���mse dur
    inorder(root->left, words); // Sol alt a�ac� gez
    words.push_back(root->data); // Mevcut d���mdeki veriyi ekle
    inorder(root->right, words); // Sa� alt a�ac� gez
}

// Kelimeleri temizler, harf olmayan karakterleri kald�r�r ve k���k harfe �evirir
string cleanWord(const string& word) {
    string cleaned;
    for (char c : word) {
        if (isalpha(c)) cleaned += tolower(c); // Yaln�zca harfleri al ve k���k harfe �evir
    }
    return cleaned;
}

// En s�k ge�en K kelimeyi bulur
vector<WordFreq> GetMostFrequentKWords(string filename, int k) {
    cout << "Info: Opening file '" << filename << "'..." << endl; // Dosya a��l�yor mesaj�
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Failed to open file '" << filename << "'!" << endl; // Hata mesaj�
        return {};
    }

    AVLNode* root = nullptr; // AVL a�ac� k�k�
    map<string, int> wordMap; // Kelime-frekans haritas�

    cout << "Info: Reading file..." << endl; // Dosya okuma mesaj�
    string word;
    while (file >> word) {
        string cleanedWord = cleanWord(word); // Kelimeyi temizle
        if (cleanedWord.length() > 2) { // 2 karakterden k�sa kelimeleri atla
            wordMap[cleanedWord]++;
        }
    }

    cout << "Info: File read successfully. Building AVL tree..." << endl;
    file.close(); // Dosyay� kapat
    cout << "Info: File '" << filename << "' closed." << endl;

    // Kelime-frekans �iftlerini AVL a�ac�na ekle
    for (const auto& pair : wordMap) {
        WordFreq wf(pair.first);
        wf.freq = pair.second;
        root = insert(root, wf);
    }

    // AVL a�ac�ndan s�ral� kelime-frekans �iftlerini al
    vector<WordFreq> allWords;
    inorder(root, allWords);

    // Kelimeleri s�kl��a ve alfabetik s�raya g�re s�rala
    sort(allWords.begin(), allWords.end(), [](const WordFreq& a, const WordFreq& b) {
        if (a.freq == b.freq) return a.word < b.word; // Frekans e�itse alfabetik s�raya g�re
        return a.freq > b.freq; // Frekans azalan s�rada
        });

    // �lk K kelimeyi al
    vector<WordFreq> result;
    for (int i = 0; i < k && i < (int)allWords.size(); i++) {
        result.push_back(allWords[i]);
    }

    cout << "Info: Successfully retrieved the top " << k << " most frequent words." << endl;
    return result;
}
