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

// Bir düðümün yüksekliðini döner
int height(AVLNode* node) {
    if (node == nullptr) return 0; // Boþ düðümse yükseklik 0
    return node->height;
}

// Bir düðümün denge faktörünü hesaplar
int getBalance(AVLNode* node) {
    if (node == nullptr) return 0; // Boþ düðümde denge faktörü 0
    return height(node->left) - height(node->right);
}

// Bir düðümün yüksekliðini günceller
void updateHeight(AVLNode* node) {
    if (node) node->height = 1 + max(height(node->left), height(node->right));
}

// Saða Döndürme (Right Rotate)
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left; // Y'nin sol alt düðümü X olur
    AVLNode* T2 = x->right; // X'in sað alt aðacýný tut

    // Saða döndürme iþlemi
    x->right = y;
    y->left = T2;

    // Yükseklikleri güncelle
    updateHeight(y);
    updateHeight(x);

    return x; // Yeni kökü döndür
}

// Sola Döndürme (Left Rotate)
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right; // X'in sað alt düðümü Y olur
    AVLNode* T2 = y->left; // Y'nin sol alt aðacýný tut

    // Sola döndürme iþlemi
    y->left = x;
    x->right = T2;

    // Yükseklikleri güncelle
    updateHeight(x);
    updateHeight(y);

    return y; // Yeni kökü döndür
}

// AVL Aðacýna bir WordFreq ekler
AVLNode* insert(AVLNode* node, const WordFreq& wf) {
    if (node == nullptr) return new AVLNode(wf); // Boþ düðümse yeni düðüm oluþtur

    // Kelime sýrasýna göre ekleme yapýlýr
    if (wf.word < node->data.word) {
        node->left = insert(node->left, wf); // Sol tarafa ekle
    }
    else if (wf.word > node->data.word) {
        node->right = insert(node->right, wf); // Sað tarafa ekle
    }
    else {
        // Eðer kelime zaten varsa, frekansýný güncelle
        node->data.freq = wf.freq;
        return node;
    }

    // Yüksekliði güncelle
    updateHeight(node);

    // Denge faktörünü kontrol et ve AVL dengesini saðla
    int balance = getBalance(node);

    // Sol-Sol durumu
    if (balance > 1 && wf.word < node->left->data.word) return rightRotate(node);

    // Sað-Sað durumu
    if (balance < -1 && wf.word > node->right->data.word) return leftRotate(node);

    // Sol-Sað durumu
    if (balance > 1 && wf.word > node->left->data.word) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Sað-Sol durumu
    if (balance < -1 && wf.word < node->right->data.word) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node; // Dengelenmiþ düðümü döndür
}

// AVL aðacýný sýralý bir þekilde dolaþýr
void inorder(AVLNode* root, vector<WordFreq>& words) {
    if (root == nullptr) return; // Boþ düðümse dur
    inorder(root->left, words); // Sol alt aðacý gez
    words.push_back(root->data); // Mevcut düðümdeki veriyi ekle
    inorder(root->right, words); // Sað alt aðacý gez
}

// Kelimeleri temizler, harf olmayan karakterleri kaldýrýr ve küçük harfe çevirir
string cleanWord(const string& word) {
    string cleaned;
    for (char c : word) {
        if (isalpha(c)) cleaned += tolower(c); // Yalnýzca harfleri al ve küçük harfe çevir
    }
    return cleaned;
}

// En sýk geçen K kelimeyi bulur
vector<WordFreq> GetMostFrequentKWords(string filename, int k) {
    cout << "Info: Opening file '" << filename << "'..." << endl; // Dosya açýlýyor mesajý
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Failed to open file '" << filename << "'!" << endl; // Hata mesajý
        return {};
    }

    AVLNode* root = nullptr; // AVL aðacý kökü
    map<string, int> wordMap; // Kelime-frekans haritasý

    cout << "Info: Reading file..." << endl; // Dosya okuma mesajý
    string word;
    while (file >> word) {
        string cleanedWord = cleanWord(word); // Kelimeyi temizle
        if (cleanedWord.length() > 2) { // 2 karakterden kýsa kelimeleri atla
            wordMap[cleanedWord]++;
        }
    }

    cout << "Info: File read successfully. Building AVL tree..." << endl;
    file.close(); // Dosyayý kapat
    cout << "Info: File '" << filename << "' closed." << endl;

    // Kelime-frekans çiftlerini AVL aðacýna ekle
    for (const auto& pair : wordMap) {
        WordFreq wf(pair.first);
        wf.freq = pair.second;
        root = insert(root, wf);
    }

    // AVL aðacýndan sýralý kelime-frekans çiftlerini al
    vector<WordFreq> allWords;
    inorder(root, allWords);

    // Kelimeleri sýklýða ve alfabetik sýraya göre sýrala
    sort(allWords.begin(), allWords.end(), [](const WordFreq& a, const WordFreq& b) {
        if (a.freq == b.freq) return a.word < b.word; // Frekans eþitse alfabetik sýraya göre
        return a.freq > b.freq; // Frekans azalan sýrada
        });

    // Ýlk K kelimeyi al
    vector<WordFreq> result;
    for (int i = 0; i < k && i < (int)allWords.size(); i++) {
        result.push_back(allWords[i]);
    }

    cout << "Info: Successfully retrieved the top " << k << " most frequent words." << endl;
    return result;
}
