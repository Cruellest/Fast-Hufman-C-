#include <cstddef>
#include <cstdint>
#include <vector>

using std::vector;

/* Nó de uma árvore de Huffman */
class Node {
private:
  int f;     // Frequency
  uint8_t c; // Code
  Node *l;   // Left child
  Node *r;   // Right child
  
public:
  Node(int f, uint8_t c, Node *l = nullptr, Node *r = nullptr); // Construtor
  Node(int f = 0, Node *l = nullptr, Node *r = nullptr);        // Construtor
  int freq() const;     // Devolve a frequência do caractere
  uint8_t code() const; // Devolve o código do caractere
  Node *left();         // Devolve o filho esquerdo
  Node *right();        // Devolve o filho direito
  bool leaf() const;    // Devolve true se é folha e false caso contrário
};


/* Lista de min-prioridades (min-heap) com nós de uma árvore de
   Huffman, utilizando a frequência como chave */
class MinHeap {
private:
  vector<Node *> v;                    // Elementos
  void up(int i);             // Sobe
  void down(int i);           // Desce
  void swap(int i, int j);
  int parent(int i); // Pai
  int left(int i);   // Filho esquerdo
  int right(int i);  // Filho direito

public:
  MinHeap();            // Construtor padrão
  ~MinHeap();           // Destrutor padrão
  size_t size();        // Tamanho da heap
  void insert(Node *n); // Inserir elemento
  Node *extract();      // Remover (menor) elemento
};

int Node::freq() const{
  return f;
}

MinHeap::MinHeap(){
  //TODO: implementar (constroi_max_heap)
  for (int i = v.size()/2 - 1; i >= 0; i--) {
    down(i);
  }
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