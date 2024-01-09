// Universidade Federal de Alfenas
// Professor: Paulo Bressan
// Alunos: Nycole Paulino Santos RA: 2023.1.08.044
// Gustavo Rodrigues RA: 2023.1.08.034

// Objetivo: : adquirir conhecimentos de manipulação de imagens em formato de
// matrizes implementando funcionalidades que geram novas imagens a partir de
// imagens lidas de arquivos-texto em formato PGM.

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Declaração da matriz bidimensional de inteiros com dimensões 1000x1000.
typedef int tImagem[1000][1000];

// Variavel utilizada para armazenar mensagens de erro.
string erro;

// Declaração da função que irá inverter a cor da imagem de entrada.
int negativa(tImagem Img_saida, tImagem Img_entrada, int linhas, int col, int tons) {
    //Inicialização do for para trocar os tons da imagem entre coluna e linha
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < col; j++) {
        Img_saida[i][j] = tons - Img_entrada[i][j];
        }
    }
    return 0;
}

// Delaração da função passagem baixa, onde o obejtivo é suavizar a imagem.
void FiltroPB(int linhas, int col, tImagem Img_saida, tImagem Img_entrada) {
    int mat[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    int somamat = 9;

    for (int i = 1; i < linhas - 1; i++) {
        for (int j = 1; j < col - 1; j++) {
        int soma = 0;
        for (int k = -1; k <= 1; k++) {
            for (int l = -1; l <= 1; l++) {
            soma += Img_entrada[i + k][j + l] * mat[k + 1][l + 1];
            }
        }
        Img_saida[i][j] = soma / somamat;
        }
    }
}

// Declarção da função que reduz a resolução da imagem, fazendo com que ela
// fique menor e mais compactada.
void Icone(tImagem Img_saida, tImagem Img_entrada, int *linhas, int *col) {
    int lines_part = (*linhas) / 64,
        columns_part = (*col) / 64,
        media = 0;
    //Inicialização do for para diminuir o tamanho da imagem criando um intem/ícone
    for (int i = 0; i < *linhas; i += lines_part) {
        for (int j = 0; j < *col; j += columns_part) {
            media = 0;
            for (int k = i; k < i + lines_part; k++) {
                for (int l = j; l < j + columns_part; l++) {
                    media += Img_entrada[k][l];
                }
            }
            media = media / (lines_part * columns_part);
            Img_saida[i / lines_part][j / columns_part] = media;
        }
    }
    //Declaração do tamanho da imagem, para sair em formato 64x64.
    *linhas = 64;
    *col = 64;
}

// Função que representa o processo de deslocando da imagem de entrada para a esqueda.
void Esquerda(int *linhas, int *col, tImagem Img_saida, tImagem Img_entrada) {
    //inicialização do for para mover a imagem através das colunas e linhas.
    for (int i = 0, j = *col; j >= 0; i++, j--){
        for (int k = 0; k < *linhas; k++){
            Img_saida[i][k] = Img_entrada[k][j];
        }
    }
    //As linhas recebem as colunas e vise-versa para o deslocamento.
    int temp = *linhas;
    *linhas = *col;
    *col = temp;
}

// Função que representa o processo de deslocando da imagem de entrada para a direita.
void Direita(int *linhas, int *col, tImagem Img_saida, tImagem Img_entrada) {
    for (int i = 0; i < *col; i++){
        for (int k = 0, j = *linhas; k < *linhas; k++, j--){
            Img_saida[i][k] = Img_entrada[j][i];
        }
    }
    //As colunas recebem as linhas e vise-versa para o deslocamento.
    int temp = *linhas;
    *linhas = *col;
    *col = temp;
}

// Função para escurecer ou clarear a imagem
void Brilho(tImagem Img_saida, tImagem Img_entrada, int linhas, int col,
    int valor, int opcao3) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < col; j++) {
        Img_saida[i][j] = Img_entrada[i][j];
        if (opcao3 == 1) {
            Img_saida[i][j] += valor;
        }
        if (Img_saida[i][j] > 255) {
            Img_saida[i][j] = 255;
        } else if (opcao3 == 2) {
            Img_saida[i][j] = Img_entrada[i][j] - valor;
            if (Img_saida[i][j] < 0) {
            Img_saida[i][j] = 0;
            }
        }
        }
    }
}

// Função que converte a imagem de entrada para uma versão binária.
void binarizar(tImagem Img_saida, tImagem Img_entrada, int linhas, int col, int limite) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < col; j++) {
        if (Img_entrada[i][j] > limite) {
            Img_saida[i][j] = 255; // Branco
        } else {
            Img_saida[i][j] = 0; // Preto
        }
        }
    }
}

// Função utilizada para fazer o carregamento da imagem em PGM.
int carregaPGM(string nome, tImagem img, int *lin, int *col, int *tons) {
    string tipo;
    ifstream arquivo(nome);

    //abertura do arquivo
    if (!arquivo.is_open()) {
        erro = "Erro: Arquivo não encontrado.";
        return 1;
    }

    arquivo >> tipo;
    if (tipo != "P2") {
        erro = "Erro: Arquivo não tem formato P2.";
        return 2;
    }

    arquivo >> *col >> *lin >> *tons;
    for (int i = 0; i < *lin; i++) {
        for (int j = 0; j < *col; j++) {
        arquivo >> img[i][j];
        }
    }
    //fechamento do arquivo
    arquivo.close();
    return 0;
}

// Função onde salva a imagem em formato PGM.
int salvaPGM(string nome, tImagem img, int lin, int col, int tons) {
    ofstream arquivo(nome);
    //Abertura do arquivo
    if (!arquivo.is_open()) {
        erro = "Erro: Arquivo não encontrado.";
        return 1;
    }

    arquivo << "P2" << endl << col << " " << lin << endl << tons << endl;
    for (int i = 0; i < lin; i++) {
        for (int j = 0; j < col; j++) {
        arquivo << img[i][j] << " ";
        }
        arquivo << endl;
    }
    //Fechamento do arquivo.
    arquivo.close();
    return 0;
}

int main() {
    tImagem Img_entrada, Img_saida;
    int col, linhas, tons;
    int opcao, limite, valor, opcao3;
    string arq_entrada, arq_saida;

    // Leitura do arquivo de entrada da imagem.
    cout << "\nNome da imagem de entrada: ";
    cin >> arq_entrada;
    arq_entrada = arq_entrada + ".pgm";
    if (carregaPGM(arq_entrada, Img_entrada, &linhas, &col, &tons) != 0) {
        cout << "\nErro ao carregar a imagem."<< endl;
        return 1;
    }

    // Opções para o Usuário.
    cout << "Escolha uma das opções abaixo:" << endl;
    cout << "0 - Sair" << endl;
    cout << "1 - Negativa" << endl;
    cout << "2 - Binarizar" << endl;
    cout << "3 - Ajuste de Brilho" << endl;
    cout << "4 - Rotacionar" << endl;
    cout << "5 - Iconizar" << endl;
    cout << "6 - Filtro Passa-Baixa" << endl;
    cin >> opcao;

    while (opcao != 0) {
        if (opcao == 1) {
            negativa(Img_saida, Img_entrada, linhas, col, tons);
        } else if (opcao == 2) {
            cout << "Digite o limite para binarizar a imagem: " << endl;
            cin >> limite;
            binarizar(Img_saida, Img_entrada, linhas, col, limite);
        } else if (opcao == 3) {
            cout << "\tEscolha a opção entre escurecer ou clarear a imagem: " << endl;
            cout << "1 - Clarear" << endl;
            cout << "2 - Escurecer" << endl;
            cin >> opcao3;
            cout << "\tDigite um valor entre 1 a 255: ";
            cin >> valor;
            Brilho(Img_saida, Img_entrada, linhas, col, valor, opcao3);
        } else if (opcao == 4) {
            char op;
            cout << "Escolha uma opção para rotacionar: " << endl;
            cout << "1. Esquerda" << endl;
            cout << "2. Direita" << endl;
            cin >> op;
            if (op == '1') {
                Esquerda(&linhas, &col, Img_saida, Img_entrada);
            } else if (op == '2') {
                Direita(&linhas, &col, Img_saida, Img_entrada);
            } else {
                cout << "Opção inválida." << endl;
                return 1;
            }
        } else if (opcao == 5) {
            Icone(Img_saida, Img_entrada, &linhas, &col);
        } else if (opcao == 6) {
            FiltroPB(linhas, col, Img_saida, Img_entrada);
        } else {
            cout << "Opção inválida." << endl;
            return 1;
        }

        cout << "Nome da imagem de saída: ";
        cin >> arq_saida;
        arq_saida = arq_saida + ".pgm";
        if (salvaPGM(arq_saida, Img_saida, linhas, col, tons) != 0) {
            cout << "\n" + erro + "\n";
            return 1;
        }

        // Opções para o Usuário.
        cout << "\n\nEscolha uma das opções abaixo:" << endl;
        cout << "0 - Sair" << endl;
        cout << "1 - Negativa" << endl;
        cout << "2 - Binarizar" << endl;
        cout << "3 - Ajuste de Brilho" << endl;
        cout << "4 - Rotacionar" << endl;
        cout << "5 - Iconizar" << endl;
        cout << "6 - Filtro Passa-Baixa" << endl;
        cin >> opcao;
        if(opcao != 0){
            // reabrindo a imagem para ler os valores corretos de linhas e colunas novamente caso o usuário deseje continuar
            if (carregaPGM(arq_entrada, Img_entrada, &linhas, &col, &tons) != 0) {
                cout << "\nErro ao carregar a imagem de entrada.\n";
                return 1;
            }
        }
    }
    return 0;
}