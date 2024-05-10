#include <cstdio>
#include <map>
#include <cstddef>
#include <cstdint>
#include <climits>
#include <string>
#include <vector>

using namespace std;

class Lista_Caracteres{

    private:
    FILE *file;

    public:
    map<char,int> letras;

    Lista_Caracteres(FILE *f){
        file = f;
        contarLetras();
    };

    void contarLetras(){
        int ascii;
        char letra;
        while((ascii = fgetc(file)) != EOF){

        letra = (char)ascii;

            if (letras.find(letra) != letras.end()){
                letras[letra] = letras[letra] + 1;
            }
            else{
                letras[letra] = 1;
            }
            
        }
    }

void printLetras(){
    for(const auto &par : letras) {
        printf("Letra:  %c, Frequencia: %d\n", par.first, par.second);
    }
}

};


/* Nó de uma árvore de Huffman */
class Node {
private:
  int f;     // Frequency
  char c; // Code
  Node *l;   // Left child
  Node *r;   // Right child
  
public:
  Node(int f, char c, Node *l = nullptr, Node *r = nullptr); // Construtor
  Node(int f = 0, Node *l = nullptr, Node *r = nullptr);        // Construtor
  int freq() const;     // Devolve a frequência do caractere
  uint8_t code() const; // Devolve o código do caractere
  Node *left();         // Devolve o filho esquerdo
  Node *right();        // Devolve o filho direito
  bool leaf() const;    // Devolve true se é folha e false caso contrário
};


Node::Node(int f, char c, Node *l, Node *r){
    Node::f = f;
    Node::c = c;
    Node::l = nullptr;
    Node::r = nullptr;
}

int Node::freq() const{
  return f;
}

uint8_t Node::code() const{
    return c;
}

Node* Node::left(){
    return l;
}

Node* Node::right(){
    return r;
}

bool Node::leaf() const{
    return false;
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
    vector<Node *> v;                    // Elementos
  MinHeap(vector<Node *> v);            // Construtor padrão
  ~MinHeap();           // Destrutor padrão
  int size();        // Tamanho da heap
  void insert(Node *n); // Inserir elemento
  Node *extract();      // Remover (menor) elemento
  void escreve(const string& prefixo, int i);
};

MinHeap::MinHeap(vector<Node *> v){
    MinHeap::v = v;
  //TODO: implementar (constroi_max_heap)
  for (int i = v.size()/2 - 1; i >= 0; i--) {
    down(i);
  }
}

int MinHeap::size(){
    return v.size();
}

MinHeap::~MinHeap() {
}

int MinHeap::parent(int i) {
  return (i - 1) / 2;
}

int MinHeap::left(int i) {
  return 2 * (i + 1) - 1;
}

int MinHeap::right(int i) {
  return 2 * (i + 1);
}

void MinHeap::swap(int i, int j) {
  Node * aux = v[i];
  v[i] = v[j];
  v[j] = aux;
}

void MinHeap::down(int i){
  //printf("entrou em down");
  //printf("%d", i);
  //TODO: implementar
  int e, d, maior;
  e = left(i);
  d = right(i);
  if (e > (int) v.size() && v[e]->freq() < v[i]->freq())
    maior = e;
  else
    maior = i;
  if (d > (int) v.size() && v[d]->freq() < v[maior]->freq())
    maior = d;
  if (maior != i) {
    swap(i, maior);
    down(maior);
  }
}

void MinHeap::up(int i) {
  while (v[parent(i)] > v[i]) {
    swap(i, parent(i));
    i = parent(i);
  }
}

void MinHeap::escreve(const string& prefixo, int i) {
  if (i < (int) v.size()) {
    bool ehEsquerdo = i % 2 != 0;
    bool temIrmao = i < (int) v.size()-1;
    
    printf(prefixo.c_str());
    printf(ehEsquerdo and temIrmao ? "├──" : "└──" );

    printf("%d\n", v[i]->freq());
      
    escreve(prefixo + (ehEsquerdo ? "│   " : "    "), left(i));
    escreve(prefixo + (ehEsquerdo ? "│   " : "    "), right(i));
  }
}

int main(int argc, char** argv)
{

    FILE *original;
    original= fopen("original.txt", "rb");

    Lista_Caracteres lista(original);

    lista.printLetras();
    printf("depois de lista");
    vector<Node *> vectorNode;

    for(const auto &letra : lista.letras) {
        //printf("entrou no loop");
        Node *no = new Node(letra.second, letra.first, nullptr, nullptr);
        vectorNode.push_back(no);
    }

    MinHeap heap(vectorNode);

    for(int i = 0; i < heap.size(); i++){
        printf("\n\n %c\n", heap.v[i]->code());
    }
    
    heap.escreve(" ", 0);

    return 0;
}