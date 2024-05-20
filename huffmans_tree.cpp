#include <cstdio>
#include <map>
#include <cstddef>
#include <cstdint>
#include <climits>
#include <string>
#include <vector>

using namespace std;

class Node {
private:
    int f;     // Frequency
    uint8_t c; // Code
    Node *l;   // Left child
    Node *r;   // Right child

public:
    Node(int f, uint8_t c, Node *l = nullptr, Node *r = nullptr); // Construtor
    Node(int f = 0, Node *l = nullptr, Node *r = nullptr);     // Construtor
    int freq() const;     // Devolve a frequência do caractere
    uint8_t code() const; // Devolve o código do caractere
    Node* left();         // Devolve o filho esquerdo
    Node* right();        // Devolve o filho direito
    bool leaf() const;    // Devolve true se é folha e false caso contrário
};

Node::Node(int f, uint8_t c, Node *l, Node *r) : f(f), c(c), l(l), r(r) {}

Node::Node(int f, Node *l, Node *r) : f(f), c('\0'), l(l), r(r) {}

int Node::freq() const {
    return f;
}

uint8_t Node::code() const {
    return c;
}

Node* Node::left() {
    return l;
}

Node* Node::right() {
    return r;
}

bool Node::leaf() const {
    return l == nullptr && r == nullptr;
}

/* Lista de min-prioridades (min-heap) com nós de uma árvore de
   Huffman, utilizando a frequência como chave */
class MinHeap {
private:
    void up(int i);             // Sobe
    void down(int i);           // Desce
    void swap(int i, int j);
    int parent(int i); // Pai
    int left(int i);   // Filho esquerdo
    int right(int i);  // Filho direito

public:
    vector<Node*> v;                    // Elementos
    MinHeap(vector<Node*> v);            // Construtor padrão
    ~MinHeap();           // Destrutor padrão
    int size();        // Tamanho da heap
    void insert(Node *n); // Inserir elemento
    Node* extract();      // Remover (menor) elemento
    void escreve(const string& prefixo, int i);
};

MinHeap::MinHeap(vector<Node*> v) : v(v) {
    //TODO: implementar (constroi_max_heap)
    for (int i = v.size() / 2 - 1; i >= 0; i--) {
        down(i);
    }
}

Node* MinHeap::extract() {
    //TODO: implementar
    Node* menor;
    if (v.size() > 0) {
        menor = v[0]; // ou v.front()
        v[0] = v.back(); // ou v[v.size()-1]
        v.pop_back();
        down(0);
        return menor;
    }
    return nullptr;
}

void MinHeap::insert(Node *n) {
    v.push_back(n);
    up(v.size() - 1);
}

int MinHeap::size() {
    return v.size();
}

MinHeap::~MinHeap() {}

int MinHeap::parent(int i) {
    return (i - 1) / 2;
}

int MinHeap::left(int i) {
    return 2 * i + 1;
}

int MinHeap::right(int i) {
    return 2 * i + 2;
}

void MinHeap::swap(int i, int j) {
    Node* aux = v[i];
    v[i] = v[j];
    v[j] = aux;
}

void MinHeap::down(int i) {
    //printf("entrou em down");
    //printf("%d", i);
    //TODO: implementar
    int e, d, maior;
    e = left(i);
    d = right(i);
    if (e < (int) v.size() && v[e]->freq() < v[i]->freq())
        maior = e;
    else
        maior = i;
    if (d < (int) v.size() && v[d]->freq() < v[maior]->freq())
        maior = d;
    if (maior != i) {
        swap(i, maior);
        down(maior);
    }
}

void MinHeap::up(int i) {
    while (i > 0 && v[parent(i)]->freq() > v[i]->freq()) {
        swap(i, parent(i));
        i = parent(i);
    }
}

void MinHeap::escreve(const string& prefixo, int i) {
    if (i < (int) v.size()) {
        bool ehEsquerdo = i % 2 != 0;
        bool temIrmao = i < (int) v.size() - 1;

        printf(prefixo.c_str());
        printf(ehEsquerdo && temIrmao ? "├──" : "└──");

        printf("%c\n", v[i]->code());

        escreve(prefixo + (ehEsquerdo ? "│   " : "    "), left(i));
        escreve(prefixo + (ehEsquerdo ? "│   " : "    "), right(i));
    }
}

class HuffmansTree {
private:
    MinHeap minHeap;
    Node* root; //raiz da arvore
    void escreve(Node* node, const string& prefixo, bool temIrmao); // Método auxiliar para imprimir a árvore

public:
    HuffmansTree(MinHeap minHeap);
    ~HuffmansTree();
    void escreve();
    void deletaA(Node *node);
};

HuffmansTree::HuffmansTree(MinHeap minHeap) : minHeap(minHeap) {
    Node* no;
    while (this->minHeap.size() > 1) {
        Node* left = this->minHeap.extract();
        Node* right = this->minHeap.extract();
        int combinedFreq = left->freq() + right->freq();
        no = new Node(combinedFreq, left, right);
        this->minHeap.insert(no);
    }
    if (this->minHeap.size() == 1){
      root = this->minHeap.extract();
    }
}

HuffmansTree::~HuffmansTree() {

    deletaA(root);

}

void HuffmansTree::deletaA(Node *node){

    if (node != nullptr){
      deletaA(node->left());
      deletaA(node->right());
      delete node;
    }
}

void HuffmansTree::escreve(Node *node, const string &prefixo, bool temIrmao){

  if (node != nullptr) {
        printf("%s", prefixo.c_str());
        printf(temIrmao ? "├──" : "└──");
        printf("%d\n", node->freq());
        

        string newPrefix = prefixo + (temIrmao ? "│   " : "    ");
        escreve(node->left(), newPrefix, node->right() != nullptr);
        escreve(node->right(), newPrefix, false);
    }
}

void HuffmansTree::escreve(){
  escreve(root,"", false);
}