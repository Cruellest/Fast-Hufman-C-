/**************************************************
*
* Gabriel Paes Duarte Baltazar, Maria Fernanda Colombo
* Trabalho de Estruturas de Dados
* Professor(a): Diego Rubert
*
*/

#include <cstdio>
#include <map>
#include <cstddef>
#include <cstdint>
#include <climits>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

/* Byte com buffer para leitura ou escrita bit-a-bit em arquivo. No
   modo escrita, quanto o buffer completa 8 bits, escreve 1 byte no
   arquivo. No modo leitura, quando o buffer fica vazio, lê 1 byte do
   arquivo e preenche o buffer. Utilize cada objeto desta classe apenas
   para ler ou apenas para escrever, nunca os dois (seu comportamento
   seria indefinido) */

class Bits {
private:
  FILE *file;   // Ponteiro para o arquivo sendo lido/escrito
  uint8_t b[8]; // Buffer com bits de um byte
  uint8_t n;    // Quantidade de posições ocupadas no vetor acima
  uint8_t j = 0;

public:
  Bits(FILE *file);   // Construtor padrão com o arquivo que será lido ou escrito
  uint8_t ocupados(); // Devolve quantos bits estão ocupados no buffer do byte
  uint8_t livres();   // Devolve quantos bits ainda podem ser adicionados ao buffer do byte

  // Funções do modo escrita
  void adiciona_bit(uint8_t bit); // Adiciona um bit 0 ou 1 ao buffer (escreve byte no arquivo se encher)
  void descarrega();              // Força a escrita do byte no buffer (completa com 0s, caso necessário)

  // Funções do modo leitura
  uint8_t obtem_bit(); // Obtém o próximo bit do buffer (lê um byte do arquivo se estiver vazio)
};
  
Bits::Bits(FILE *file) :
  file(file),
  n(0)
{ }

void Bits::adiciona_bit(uint8_t bit)
{
  b[n++] = bit != 0; // para usar 0/1 mesmo se bit > 1

  if (n == 8)
    descarrega();
}

void Bits::descarrega()
{
  // Implementar

  // Deve completar o byte com 0s para preencher os 8 bits, caso n < 8

  if (this->n == 0){
    return;
  }

  for(int i = this->n; i < 8; i++){
    this->b[i] = 0;
    
  }
    uint8_t byte = 0;

    for(int i = 0; i < 8; i++){
        byte <<= 1;
        byte += this->b[i];

    }
    fwrite(&byte, sizeof(byte) , 1 ,this->file);

    this->n = 0;

  // Leia sobre a função fwrite

  // No final, não esqueça de fazer n receber 0
}

uint8_t Bits::obtem_bit()
{
  // Caso n == 0, deve ler 1 byte do arquivo e colocar no buffer,
  // ou devolver 2 caso não haja mais bytes para serem lidos do arquivo
  if (n == 0) {
    uint8_t byte;
    size_t resultado = fread(&byte, sizeof(uint8_t), 1, file);

    // Se não há mais bytes para serem lidos do arquivo
    if (resultado != 1) {
      return 2;
    }

    // Preenche o buffer com os bits do byte lido
    for (int i = 0; i < 8; i++) {
      b[i] = (byte >> (7 - i)) & 1;
    }

    // Atualiza n para 8 pois acabamos de ler 1 byte (8 bits)
    n = 8;
  }
    if (j >= 8){
        j = 0;
    }

    n--;
  // Devolve um bit (0 ou 1) e decrementa n
  return b[j++];
}


uint8_t Bits::ocupados()
{
  return n;
}

uint8_t Bits::livres()
{
  return 8 - n;
}

/*Lista os caracteres do arquivo com suas respectivas Frequencias e Codigos, Por meio de um Map*/
class Lista_Caracteres{

    private:
    FILE *file; //Arquivo a ser lido;

    public:
    map<char,int> letras; //Map que irá armazenar as letras com suas frequencias;


    Lista_Caracteres(FILE *f){  //construtor da classe
        file = f;
        contarLetras();
    };

    void contarLetras(){ //Checa quais letras estão presentes no arquivo, caso elas ja estejam na Map, adiciona +1 na frequencia
        int ascii;       //caso o contrario adiciona mais uma entrada na Map;
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

void printLetras(){ //Função para printar letras dentro da Map;
    for(const auto &par : letras) {
        printf("Letra:  %c, Frequencia: %d\n", par.first, par.second);
    }
}

};

/* Nó de uma árvore de Huffman */
class Node {
private:
    int f;     // Frequencia;
    uint8_t c; // Codigo;
    Node *l;   // Filho Esquerdo;
    Node *r;   // Filho Direito;

public:
    Node(int f, uint8_t c, Node *l = nullptr, Node *r = nullptr); // Construtor;
    Node(int f = 0, Node *l = nullptr, Node *r = nullptr);        // Construtor;
    int freq() const;     // Getter para Frequencia do Caractere;
    uint8_t code() const; // Getter para o Codigo do Caractere;
    void codeSet(uint8_t newcode); //Setter para o Codigo do Caractere;
    Node* left() const;   // Getter Filho Esquerdo;
    Node* right() const;  // Getter Filho Direito;
    void setLeft(Node* left);   // Setter Filho Esquerdo;
    void setRight(Node* right); // Setter Filho Direito;
    bool leaf() const;    // Retorna True se for folha e False caso contrario;
};

Node::Node(int f, uint8_t c, Node *l, Node *r) : f(f), c(c), l(l), r(r) {}

Node::Node(int f, Node *l, Node *r) : f(f), c('\0'), l(l), r(r) {}

int Node::freq() const {
    return f;
}

uint8_t Node::code() const {
    return c;
}

void Node::codeSet(uint8_t newCode){
    this->c = newCode;
}

Node* Node::left() const {
    return l;
}

Node* Node::right() const {
    return r;
}

void Node::setLeft(Node* left) {
    l = left;
}

void Node::setRight(Node* right) {
    r = right;
}

bool Node::leaf() const {
    return l == nullptr && r == nullptr;
}


/* Lista de min-prioridades (min-heap) com nós de uma árvore de
   Huffman, utilizando a frequência como chave */
class MinHeap {
private:
    void up(int i);             // Sobe;
    void down(int i);           // Desce;
    void swap(int i, int j);    //Troca;
    int parent(int i);          // Pai;
    int left(int i);            // Filho esquerdo;
    int right(int i);           // Filho direito;

public:
    vector<Node*> v;                            // Elementos
    MinHeap(vector<Node*> v);                   // Construtor padrão;
    ~MinHeap();                                 // Destrutor padrão;
    int size();                                 // Tamanho da heap;
    void insert(Node *n);                       // Inserir elemento;
    Node* extract();                            // Remover (menor) elemento;
    void escreve(const string& prefixo, int i); // Escreve todos os elementos em formato de arvore binaria;
};

MinHeap::MinHeap(vector<Node*> v) : v(v) {
    for (int i = v.size() / 2 - 1; i >= 0; i--) {
        down(i);
    }
}

Node* MinHeap::extract() {
    Node* menor;
    if (v.size() > 0) {
        menor = v[0];
        v[0] = v.back();
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

        printf("%s",prefixo.c_str());
        printf(ehEsquerdo && temIrmao ? "├──" : "└──");

        printf("%c\n", v[i]->code());

        escreve(prefixo + (ehEsquerdo ? "│   " : "    "), left(i));
        escreve(prefixo + (ehEsquerdo ? "│   " : "    "), right(i));
    }
}

/*Arvore de huffman com contrutor e operações basicas alem de funçoes para escreve-la
  e gerar os codigos de huffman para cada letra*/

class HuffmansTree {
private:
    MinHeap *minHeap;
    Node* root;                                                                         //Raiz da arvore;
    void escreve(Node* node, const string& prefixo, bool temIrmao);                     //Método auxiliar para imprimir a árvore;
    void gerarTabelaCodigos(Node *node, string byte, map<char,string> &tabelaCodigos);  //Método auxiliar para gerar codigos huffman para cada letra individualmente;
    friend class Compact;                                                               //Classe para compactar arvore;
    friend class Decompact;                                                             //Classe para descompactar arvore;

public:
    HuffmansTree(MinHeap *minHeap);         //Construtor da Classe;
    HuffmansTree(Node *no);                 //Construtor extra (Essencial na Reconstrução);
    ~HuffmansTree();                        //Destrutor da classe
    void escreve();                         //Metodo para a imprimir a arvore
    void deletaA(Node *node);               //Metodo auxiliar para Deletar a arvore
    map<char,string> gerarTabelaCodigos();  //Metodo para retornar a tabela gerada.

};

HuffmansTree::HuffmansTree(MinHeap *minHeap) : minHeap(minHeap) {
    Node* no;
    while (this->minHeap->size() > 1) {
        Node* left = this->minHeap->extract();
        Node* right = this->minHeap->extract();
        int combinedFreq = left->freq() + right->freq();
        no = new Node(combinedFreq, left, right);
        this->minHeap->insert(no);
    }
    if (this->minHeap->size() == 1){
      root = this->minHeap->extract();
    }
}

HuffmansTree::HuffmansTree(Node *no): minHeap(nullptr){
    this->root = no;

};

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
        if (node->code() != 0){
            printf("%c\n", node->code());
        }
        else{
            printf("%d\n",node->freq());
        }
        string newPrefix = prefixo + (temIrmao ? "│   " : "    ");
        escreve(node->left(), newPrefix, node->right() != nullptr);
        escreve(node->right(), newPrefix, false);
    }
}

void HuffmansTree::escreve(){
  escreve(root,"", false);
}

map<char,string> HuffmansTree::gerarTabelaCodigos(){
    
    string byte;
    map<char,string> tabelaCodigos; //Map com a função de guardar os codigos das letras
    gerarTabelaCodigos(root,byte,tabelaCodigos);
    return tabelaCodigos;

    
}

void HuffmansTree::gerarTabelaCodigos(Node* node, string byte, map<char, string>& tabelaCodigos) {
    if (node == nullptr) {
        return;  // Se o codigo é null retornar recursivamente
    }

    if (node->leaf()) {  //Se for uma folha (caractere)
        tabelaCodigos[node->code()] = byte;  //Adicione o codigo ao byte
    } else {
        // Recursivamente retorne para a direita e esquerda
        gerarTabelaCodigos(node->left(), byte + "0", tabelaCodigos);  // Esquerda: append 0
        gerarTabelaCodigos(node->right(), byte + "1", tabelaCodigos);  // Direita: append 1
    }
}
 /*Compactador para o arquivo .txt realizando todas as funções por meio de metodos internos*/
class Compact
{
private:
    FILE *fileIn;                   //Arquivo de Entrada;
    FILE *fileOut;                  //Arquivo de Saida;
    Lista_Caracteres *lista;        //Lista com caracteres presentes no arquivo de entrada;
    MinHeap *heap;                  //Minheap;
    Bits *out;                      //Buffer de saida;
    HuffmansTree *huff;             //Arvore de huffman criada com base na MinHeap;
    map<char,string> tabelaCodigos; //Map para armazenar os codigos de cada letra;

    void declararVariaveis();                                               //Declara variaveis para compactar o arquivo;
    void montarCabecalho();                                                 //Monta o cabeçalho do arquivo compactado;
    void extrairLetrasDisponiveis(Node *node,FILE *fileOut);                //Extrai as letras disponiveis no arquivo de entrada;
    void adicionarCodigoLetras(Node *node, Bits *out);                      //Adiciona o codigo das letras no cabeçalho;
    void debugOptions();                                                    //Ativa os logs de debug na execução;
    void insertData();                                                      //Insere os dados das letras já codificados no arquivo;
    void checarArquivos(string arquivoDeEntrada,string arquivoDeSaida);     //Checa a existencia dos arquivos de entrada e saida;

public:
    Compact(string arquivoDeEntrada,string arquivoDeSaida, bool debugFlag); //Construtor da classe;
    ~Compact();                                                             //Destrutor da classe;
    
};

Compact::~Compact()
{
    delete lista;
    delete heap;
    fclose(this->fileIn);
    fclose(this->fileOut);
    delete out;
    delete huff;
}

void Compact::debugOptions(){
    this->lista->printLetras();
    this->heap->escreve(" ",0);
    this->huff->escreve();
    for(const auto &par : this->tabelaCodigos) {
    printf("Letra:  %c, codigo Huffman: %s\n", par.first, par.second.c_str());
    }

}

void Compact::checarArquivos(string arquivoDeEntrada,string arquivoDeSaida){
    this->fileIn = fopen(arquivoDeEntrada.c_str(),"rb");
    if(this->fileIn == nullptr){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }
    
    this->fileOut = fopen(arquivoDeSaida.c_str(),"wb");
        if(this->fileOut == nullptr){
        printf("Erro na Criação do arquivo de saida\n");
        exit(1);
    }
}

void Compact::declararVariaveis(){
    this->lista = new Lista_Caracteres(this->fileIn);
    vector<Node *> vectorNode;
    Node *no;
    for(const auto &letra : this->lista->letras) {
        no = new Node(letra.second, letra.first, nullptr, nullptr);
        vectorNode.push_back(no);
    }
    this->heap = new MinHeap(vectorNode);
    this->out = new Bits(this->fileOut);
    this->huff = new HuffmansTree(this->heap);

}

void Compact::insertData(){
    fseek(this->fileIn, 0, SEEK_SET);
    int ascii;
    char letra;
    while ((ascii = fgetc(this->fileIn)) != EOF) {
    letra = (char)ascii;
    string codigo = this->tabelaCodigos[letra];
    for (char bit : codigo) {
    this->out->adiciona_bit(bit == '1');
    }
}
    this->out->descarrega();
}

Compact::Compact(string arquivoDeEntrada,string arquivoDeSaida, bool debugFlag)
{
    checarArquivos(arquivoDeEntrada,arquivoDeSaida);
    declararVariaveis();
    this->tabelaCodigos = this->huff->gerarTabelaCodigos();

    if (debugFlag){
        debugOptions();
    }
    montarCabecalho();
    insertData();
    }
void Compact::extrairLetrasDisponiveis(Node *node,FILE *fileOut){
    if (node == nullptr) {
        return;  // Se o codigo é null retornar recursivamente
    }

    if (node->leaf()) {  //Se for uma folha (caractere)
        uint8_t bit = node->code();  //Adicione o codigo ao byte
        fwrite(&bit,sizeof(uint8_t),1,fileOut);

    } else {
        // Recursivamente retorne para a direita e esquerda
        extrairLetrasDisponiveis(node->left(),fileOut);  // 
        extrairLetrasDisponiveis(node->right(),fileOut);  // 1
    }
}

void Compact::montarCabecalho(){
    
    uint16_t bit = this->lista->letras.size();
    fwrite(&bit,sizeof(uint16_t),1,this->fileOut);

    uint32_t qntLetras = this->huff->root->freq();

    fwrite(&qntLetras,sizeof(uint32_t),1,this->fileOut);

    extrairLetrasDisponiveis(this->huff->root,this->fileOut);
    adicionarCodigoLetras(this->huff->root,this->out);
}

void Compact::adicionarCodigoLetras(Node *node, Bits *out) {
    if (node == nullptr) {
        return;  // Se o nó é null, retornar
    }

    if (node->leaf()){
        out->adiciona_bit(1);
        return;
    }

    else{
        out->adiciona_bit(0);
        adicionarCodigoLetras(node->left(),out);
        adicionarCodigoLetras(node->right(),out);    
    }

}


/*Classe Criada para Descompactar arquivos compactados pela função Compactar*/
class Decompact {
private:
    FILE *fileIn;                                       //Arquivo de Entrada;
    FILE *fileOut;                                      //Arquivo de Saida;
    Bits *in;                                           //Buffer de Saida;
    HuffmansTree *huff;                                 //ArvoreReconstruida;
    uint16_t qntLetras;                                 //Quantidade de caracteres diferentes no Arquivo;
    uint32_t tamanhoDoArquivo;                          //Quantidade total de caracteres no Arquivo;
    vector<char> letras;                                //Vetor auxiliar para reconstruir a arvore de Huffman;
    void reconstruirArvore(Node *no, int &cont);        //Metodo Para reconstruir a arvore;
    void escreverDados();                               //Metodo para escrever os dados no arquivo;
    void decodificarLetra(Node *no, uint32_t *cont);    //Metodo para decodificar os codigos das letras;
    void debugOption();

public:
    Decompact(string fileIn, string fileOut,bool debug);   //Construtor da classe (Ativa a decompactação);
    ~Decompact();                               //Destrutor da classe;
};

Decompact::Decompact(string fileIn, string fileOut,bool debug) {
    this->fileIn = fopen(fileIn.c_str(), "rb");
    if(this->fileIn == nullptr){
        printf("Arquivo de entrada não encontrado\n");
        exit(1);
        return;
    }
    this->fileOut = fopen(fileOut.c_str(), "wb");
    if(this->fileOut == nullptr){
        printf("Arquivo de Saida não encontrado\n");
        exit(1);
        return;
    }
    in = new Bits(this->fileIn);

    fread(&this->qntLetras, sizeof(uint16_t), 1, this->fileIn);
    fread(&tamanhoDoArquivo, sizeof(uint32_t), 1, this->fileIn);
    for (int i = 0; i < this->qntLetras; i++) {
        letras.push_back(fgetc(this->fileIn));
    }

    Node *no = new Node();
    this->huff = new HuffmansTree(no);
    int cont = 0;
    reconstruirArvore(this->huff->root, cont);
    escreverDados();

    if(debug){
        debugOption();
    }
}

void Decompact::debugOption(){
    printf("Quantidade de characteres diferentes do arquivo: %d\n", this->qntLetras);
    printf("Quantidade total de characteres no arquivo: %d\n", this->tamanhoDoArquivo);
    printf("Characteres diferentes no Arquivo: ");
    for (int i = 0; i < this->qntLetras; i++) {
        printf("%c ", this->letras[i]);
    }
    printf("\n");
    this->huff->escreve();
}

void Decompact::reconstruirArvore(Node *no, int &cont) {
    if (cont >= this->qntLetras) {
        return;
    }

    if (in->obtem_bit() == 0) {
        Node *leftNode = new Node();
        Node *rightNode = new Node();
        no->setLeft(leftNode);
        no->setRight(rightNode);
        reconstruirArvore(leftNode, cont);
        reconstruirArvore(rightNode, cont);
    } else {
        no->codeSet(this->letras[cont]);
        cont++;
    }
}

void Decompact::decodificarLetra(Node *no, uint32_t *cont) {
    if (*cont >= this->tamanhoDoArquivo) {
        return;
    }

    if (no->leaf()) {
        uint8_t temp = no->code();
        fwrite(&temp, sizeof(uint8_t), 1, this->fileOut);
        (*cont)++;
        return;
    }

    uint8_t bit = in->obtem_bit();
    if (bit == 0) {
        decodificarLetra(no->left(), cont);
    } else {
        decodificarLetra(no->right(), cont);
    }
}

void Decompact::escreverDados() {
    uint32_t cont = 0;

    while (cont < tamanhoDoArquivo) {
        decodificarLetra(huff->root, &cont);
    }
}

Decompact::~Decompact() {
    delete huff;
    delete in;
    fclose(fileIn);
    fclose(fileOut);
}

int main(int argc, char const *argv[]) {
    if (argc > 4){
        printf("Too many arguments\n");
        return 1;
    }

    else if(argc < 4){
        printf("Too few arguments\n");
        return 1;
    }

    bool debug = false; //Flag para depuração caso queira checar todas as variaveis usadas
                        //só setar como true;

    if (strcmp(argv[1],"c") == 0){
        Compact File(argv[2],argv[3],debug);

    }else if(strcmp(argv[1],"d") == 0){
        Decompact File(argv[2],argv[3],debug);
    }
    return 0;
}