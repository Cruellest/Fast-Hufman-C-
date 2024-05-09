#include <cstdio>
#include <map>

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


int main(int argc, char const *argv[])
{

    FILE *original;

    original= fopen("original.txt", "rb");
    if (original == NULL) {
        printf("Falha ao abrir o arquivo.\n");
        return 1;
    }


    Lista_Caracteres lista(original);

    lista.printLetras();

    fclose(original);


    return 0;
}
