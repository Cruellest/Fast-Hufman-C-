#include <cstdio>
#include <cstdint>

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

int main()
{
  //Test Main
  FILE *original, *copia;
  original = fopen("original.txt", "rb");
  copia = fopen("copia.txt", "wb");

  Bits in(original), out(copia);
  uint8_t bit;
  
  while ((bit = in.obtem_bit()) != 2) {
    putchar('0' + bit);
    out.adiciona_bit(bit);
  }
  putchar('\n');
  out.descarrega();

  fclose(original);
  fclose(copia);
  
  return 0;
}
  
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

    for(int i = 8; i > -1; i--){
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

  // Devolve um bit (0 ou 1) e decrementa n
  return b[--n];
}


uint8_t Bits::ocupados()
{
  return n;
}

uint8_t Bits::livres()
{
  return 8 - n;
}