//
//  palavras.c
//
//
// Created by Pedro Lopes on 15/04/2022.
//

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*Menu Help*/

int DEFAULT_GAME=1;

void help(char *programa)
{
    printf("Palavras Cruzadas.\n\n");
    printf("Utilizaçao: %s [opçoes]\n", programa);
    printf("Opçoes validas:\n");
    printf("[-h]\t\t   ajuda para o utilizador\n");
    printf("[-t [linha]x[coluna]]\t   define a dimensao do tabuleiro de jogo. O valor pre-definido vai é 9x9\n");
    printf("[-d (FILENAME)]\t   define o ficheiro dicionario a usar\n");
    printf("[-l (FILENAME)]\t   define o ficheiro com as letras restringidas, usadas no modo de jogo 3 e 4\n");
    printf("[-m (5-20)]\t   define o numero maximo de letras que o jogador pode ter na mao, so aplicável nos modos de jogo 3 e 4\n");
    printf("[-n (nn)]\t   define o numero maximo de jogadas, pre-definido valor 0\n");
    printf("[-i (FILENAME)]\t   define o nome do ficheiro que contem o tabuleiro que o jogador quer utilizar para o jogo,a dimensao deste definida previamente em -t\n");
    printf("[-j (0-4)]\t   define o modo de jogo (por omissao, o modo de jogo e %d)\n", DEFAULT_GAME);
    printf("[-o (FILENAME)]\t   define o ficheiro para escrever e guardar o tabuleiro final\n");
    printf("[-r (FILENAME)]\t   define o ficheiro para guardar o registo das jogadas possiveis, apenas para os modos de jogo 1 2 e 3. Por omissao, não ha registo\n");
}


// Pre-formatacao do tabuleiro
void pre_formatacao(int colunas_total, int linhas_total, int tamanho_tabuleiro,
                    char dados[linhas_total][colunas_total][7]) {
  int linha, coluna, linha_index, coluna_index;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  for (linha = 0; linha <= linha_index; linha++) {
    for (coluna = 0; coluna <= coluna_index; coluna++) {
      if ((coluna >= 3) && (coluna <= coluna_index) && (linha >= 0) &&
          (linha <= linha_index - 2)) {
        dados[linha][coluna][0] = '.';
        dados[linha][coluna][1] = '1';
      } else {
        dados[linha][coluna][0] = ' ';
        dados[linha][coluna][1] = '0';
      }
    }
  }
}

// Gerar tabuleiro
void gerar_tabuleiro(int colunas_total, int linhas_total, int tamanho_tabuleiro,
                     char dados[linhas_total][colunas_total][7]) {
  int linha, coluna, linha_index, coluna_index;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  for (linha = 0; linha <= linha_index; linha++) {
    for (coluna = 0; coluna <= coluna_index; coluna++) {
      // extremos $
      if ((coluna == 3 || coluna == coluna_index) &&
          (linha == 0 || linha == linha_index - 2)) {
        dados[linha][coluna][0] = '$';
      }
      // tripos 3
      else if (((coluna == 3 + ((tamanho_tabuleiro - 1) / 2)) &&
                (linha == 0 || linha == linha_index - 2)) ||
               ((coluna == 3 || coluna == coluna_index) &&
                (linha == ((tamanho_tabuleiro - 1) / 2)))) {
        dados[linha][coluna][0] = '3';
        dados[linha][coluna][1] = '3';
      }
      // duplos 2
      else if (((linha == coluna - 3) || (linha + coluna == coluna_index)) &&
               ((linha != 0) && (linha != linha_index - 2) &&
                (linha < linha_index - 2))) {
        dados[linha][coluna][0] = '2';
        dados[linha][coluna][1] = '2';
      }
      // casas proibidas #
      else if (((((coluna == 4 + ((tamanho_tabuleiro - 1) / 2)) ||
                  (coluna == 2 + ((tamanho_tabuleiro - 1) / 2)))) &&
                (linha == 1 || linha == linha_index - 3)) ||
               (((coluna == 4) || (coluna == coluna_index - 1)) &&
                ((linha == ((tamanho_tabuleiro - 1) / 2) + 1) ||
                 (linha == ((tamanho_tabuleiro - 1) / 2) - 1)))) {
        dados[linha][coluna][0] = '#';
        dados[linha][coluna][1] = '0';
      }
      // coordenadas linhas
      else if ((linha == linha_index) && (coluna >= 3)) {
        dados[linha][coluna][0] = 'A' + coluna - 3;
      }
      // coordenadas colunas
      else if (coluna == 0 && linha >= 9 && linha <= linha_index - 2) {
        dados[linha][coluna][0] = '1';
      } else if (coluna == 1 && linha >= 9 && linha <= linha_index - 2) {
        dados[linha][coluna][0] = '0' + linha - 9;
      } else if (coluna == 1 && linha >= 0 &&
                 (linha < 9 && linha <= linha_index - 2)) {
        dados[linha][coluna][0] = '0' + linha + 1;
      }
    }
  }
}

// imprimir o tabuleiro
void imprimir_tabuleiro(int colunas_total, int linhas_total,
                        char dados[linhas_total][colunas_total][7]) {
  int linha, coluna, linha_index, coluna_index;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  for (linha = 0; linha <= linha_index; linha++) {
    printf("\n");
    for (coluna = 0; coluna <= coluna_index; coluna++) {
      printf("%c", dados[linha][coluna][0]);
      if ((coluna >= 3) && (coluna <= coluna_index) && (linha >= 0) &&
          (linha <= linha_index)) {
        printf(" ");
      }
    }
  }
  printf("\n");
}

// Validacao da jogada
int validacao_da_jogada(int N_jogada, int tamanho_tabuleiro,
                        char jogada_conteudo[], int Linha_jogada,
                        char orientacao_jogada, char Coluna_jogada,
                        int colunas_total, int linhas_total,
                        char dados[linhas_total][colunas_total][7],
                        char erros[6]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra,
      validade = 0, validade_dos_caracteres = 0, validade_das_posicoes = 0,
      igualdade_caracteres = 0;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;
  //    Se é horizontal:

  if (orientacao_jogada == 'h' || orientacao_jogada == 'H') {

    //        TESTE DOS DIFERENTES CRITERIOS:

    if (N_jogada == 1) {
      // A primeira jogada esta na casa central
      if (coluna_inicial != 3 + ((tamanho_tabuleiro - 1) / 2) &&
          linha_inicial != ((tamanho_tabuleiro - 1) / 2)) {
        return 0;
      } else {
        erros[0] = '1';
      }
      // 1 : Cabe no tabuleiro
      if ((Coluna_jogada + tamanho_palavra - 1 <= 'A' + tamanho_tabuleiro) ||
          (Coluna_jogada + tamanho_palavra - 1 <= 'a' + tamanho_tabuleiro)) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 2 : validade dos caracteres (ter cuidado com onde comeca o i!)

      for (int i = 0; i < tamanho_palavra; i++) {
        int c1 = (int)jogada_conteudo[i];
        if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) {
          validade_dos_caracteres++;
        }
      }
      if (validade_dos_caracteres == tamanho_palavra) {
        validade++;
      } else {
        erros[2] = '1';
      }
      // 3 : Nao ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[linha_inicial][i + coluna_inicial + 3][0] != '#') {
          validade_das_posicoes++;
        }
      }
      if (validade_das_posicoes == tamanho_palavra) {
        validade++;
      } else {
        erros[3] = '1';
      }
      // 4ª comprimento
      if (tamanho_palavra >= 2 && tamanho_palavra <= tamanho_tabuleiro) {
        validade++;
      } else {
        erros[4] = '1';
      }

      if (validade == 4) {
        return 1;
      } else {
        return 0;
      }
    }

    else {
      // 1 : Tem pelo menos um caracter igual;
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[linha_inicial][i + coluna_inicial + 3][0] ==
            jogada_conteudo[i]) {
          igualdade_caracteres++;
        }
      }
      if (igualdade_caracteres > 0) {
        validade++;
      } else {
        erros[5] = '1';
      }
      for (int i = 0; i < tamanho_palavra; i++) {
        int c1 = (int)dados[linha_inicial][i + coluna_inicial + 3][0];
        if (((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) &&
            (dados[linha_inicial][i + coluna_inicial + 3][0] !=
             jogada_conteudo[i])) {
          return 0;
        }
      }
      // 2 : Cabe no tabuleiro
      if (Coluna_jogada + tamanho_palavra - 1 <= 'A' + coluna_index - 3 ||
          Coluna_jogada + tamanho_palavra - 1 <= 'a' + coluna_index - 3) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 3 : validade dos caracteres (ter cuidado com onde comeca o i!)

      for (int i = 0; i < tamanho_palavra; i++) {
        int c1 = (int)jogada_conteudo[i];
        if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) {
          validade_dos_caracteres++;
        }
      }
      if (validade_dos_caracteres == tamanho_palavra) {
        validade++;
      } else {
        erros[2] = '1';
      }
      // 4 : Nao ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[linha_inicial][i + coluna_inicial + 3][0] != '#') {
          validade_das_posicoes++;
        }
      }
      if (validade_das_posicoes == tamanho_palavra) {
        validade++;
      } else {
        erros[3] = '1';
      }
      // 5ª: comprimento
      if ((tamanho_palavra >= 2) && (tamanho_palavra <= tamanho_tabuleiro)) {
        validade++;
      } else {
        erros[4] = '1';
      }

      if (validade == 5) {
        return 1;
      } else {
        return 0;
      }
    }
  }
  // Se é vertical
  else if (orientacao_jogada == 'v' || orientacao_jogada == 'V') {

    if (N_jogada == 1) {
      if (coluna_inicial != 3 + ((tamanho_tabuleiro - 1) / 2) &&
          linha_inicial != ((tamanho_tabuleiro - 1) / 2)) {
        return 0;
      } else {
        erros[0] = '1';
      }
      // 1 : Cabe no tabuleiro
      if (linha_inicial + tamanho_palavra - 1 <= linha_index) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 2 : validade dos caracteres (ter cuidado com onde comeca o i!)

      for (int i = 0; i < tamanho_palavra; i++) {
        int c1 = (int)jogada_conteudo[i];
        if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) {
          validade_dos_caracteres++;
        }
      }
      if (validade_dos_caracteres == tamanho_palavra) {
        validade++;
      } else {
        erros[2] = '1';
      }
      // 3 : Nao ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[i + linha_inicial][coluna_inicial + 3][0] != '#') {
          validade_das_posicoes++;
        }
      }
      if (validade_das_posicoes == tamanho_palavra) {
        validade++;
      } else {
        erros[3] = '1';
      }
      //        4ª comprimento
      if (tamanho_palavra >= 2 && tamanho_palavra <= tamanho_tabuleiro) {
        validade++;
      } else {
        erros[4] = '1';
      }

      if (validade == 4) {
        return 1;
      } else {
        return 0;
      }
    }

    else {
      // 1 : Tem pelo menos um caracter igual;
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[i + linha_inicial][coluna_inicial + 3][0] ==
            jogada_conteudo[i]) {
          igualdade_caracteres++;
        }
      }
      if (igualdade_caracteres > 0) {
        validade++;
      } else {
        erros[5] = '1';
      }
      for (int i = 0; i < tamanho_palavra; i++) {
        int c1 = (int)dados[linha_inicial + i][coluna_inicial + 3][0];
        if (((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) &&
            (dados[i + linha_inicial][coluna_inicial + 3][0] !=
             jogada_conteudo[i])) {
          return 0;
        }
      }
      // 2 : Cabe no tabuleiro
      if (linha_inicial + tamanho_palavra - 1 <= linha_index) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 3 : validade dos caracteres (ter cuidado com onde comeca o i!)

      for (int i = 0; i < tamanho_palavra; i++) {
        int c1 = (int)jogada_conteudo[i];
        if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) {
          validade_dos_caracteres++;
        }
      }
      if (validade_dos_caracteres == tamanho_palavra) {
        validade++;
      } else {
        erros[2] = '1';
      }
      // 4 : Nao ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[i + linha_inicial][coluna_inicial + 3][0] != '#') {
          validade_das_posicoes++;
        }
      }
      if (validade_das_posicoes == tamanho_palavra) {
        validade++;
      } else {
        erros[3] = '1';
      }
      // 5ª comprimento
      if (tamanho_palavra >= 2 && tamanho_palavra <= tamanho_tabuleiro) {
        validade++;
      } else {
        erros[4] = '1';
      }

      if (validade == 5) {
        return 1;
      } else {
        return 0;
      }
    }
  } else {
    return 0;
  }
}

// Insere jogada no tabuleiro
void Insere_jogada_tabuleiro(int N_jogada, int tamanho_tabuleiro,
                             char jogada_conteudo[], int Linha_jogada,
                             char orientacao_jogada, char Coluna_jogada,
                             int colunas_total, int linhas_total,
                             char dados[linhas_total][colunas_total][7]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;

  if (orientacao_jogada == 'h' || orientacao_jogada == 'H') {
    for (int i = 0; i < tamanho_palavra; i++) {
      dados[linha_inicial][i + coluna_inicial + 3][0] = jogada_conteudo[i];
    }
  }

  if (orientacao_jogada == 'v' || orientacao_jogada == 'V') {
    for (int i = 0; i < tamanho_palavra; i++) {
      dados[i + linha_inicial][coluna_inicial + 3][0] = jogada_conteudo[i];
    }
  }
}

// Contabilizacao de Pontos
// Tem apenas em contabilizacao a palavra em si e os bonus por letra, nao inclui
// os bonus de palavra
void contabilizacao_de_pontos(int N_jogada, int tamanho_tabuleiro,
                              char jogada_conteudo[], int Linha_jogada,
                              char orientacao_jogada, char Coluna_jogada,
                              int colunas_total, int linhas_total,
                              char dados[linhas_total][colunas_total][7]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;

  if (orientacao_jogada == 'h' || orientacao_jogada == 'H') {
    for (int i = 0; i < tamanho_palavra; i++) {
      switch (tolower(jogada_conteudo[i])) {
        //               letras que contam 1
      case 'a':
      case 'e':
      case 'i':
      case 'l':
      case 'o':
      case 'r':
      case 's':
      case 't':
      case 'u':
        dados[linha_inicial][i + coluna_inicial + 3][2] = '1';
        break;
        //               letras que contam 2
      case 'd':
      case 'g':
        dados[linha_inicial][i + coluna_inicial + 3][2] = '2';
        break;
        //               letras que contam 3
      case 'b':
      case 'c':
      case 'm':
      case 'p':
        dados[linha_inicial][i + coluna_inicial + 3][2] = '3';
        break;
        //               letras que contam 4
      case 'f':
      case 'h':
      case 'v':
      case 'w':
      case 'y':
        dados[linha_inicial][i + coluna_inicial + 3][2] = '4';
        break;
        //               letras que contam 5
      case 'k':
        dados[linha_inicial][i + coluna_inicial + 3][2] = '5';
        break;
        //               letras que contam 8
      case 'j':
      case 'x':
        dados[linha_inicial][i + coluna_inicial + 3][2] = '8';
        break;
        //               letras que contam 10 (usa : porque 10 nao existe em
        //               codigo ascci,e : é o caracter aseguir a 9)
      case 'q':
      case 'z':
        dados[linha_inicial][i + coluna_inicial + 3][2] = ':';
        break;
      default:
        dados[linha_inicial][i + coluna_inicial + 3][2] = '0';
        break;
      }
    }

    for (int i = 0; i < tamanho_palavra; i++) {
      dados[linha_inicial][i + coluna_inicial + 3][3] =
          ((int)(dados[linha_inicial][i + coluna_inicial + 3][1]) - 48) *
          ((int)(dados[linha_inicial][i + coluna_inicial + 3][2]) - 48);
      if (dados[linha_inicial][i + coluna_inicial + 3][1] == '2' ||
          dados[linha_inicial][i + coluna_inicial + 3][1] == '3') {
        dados[linha_inicial][i + coluna_inicial + 3][1] = '1';
      }
    }
  }

  if (orientacao_jogada == 'v' || orientacao_jogada == 'V') {
    for (int i = 0; i < tamanho_palavra; i++) {
      switch (tolower(jogada_conteudo[i])) {
        //               letras que contam 1
      case 'a':
      case 'e':
      case 'i':
      case 'l':
      case 'o':
      case 'r':
      case 's':
      case 't':
      case 'u':
        dados[i + linha_inicial][coluna_inicial + 3][2] = '1';
        break;
        //               letras que contam 2
      case 'd':
      case 'g':
        dados[i + linha_inicial][coluna_inicial + 3][2] = '2';
        break;
        //               letras que contam 3
      case 'b':
      case 'c':
      case 'm':
      case 'p':
        dados[i + linha_inicial][coluna_inicial + 3][2] = '3';
        break;
        //               letras que contam 4
      case 'f':
      case 'h':
      case 'v':
      case 'w':
      case 'y':
        dados[i + linha_inicial][coluna_inicial + 3][2] = '4';
        break;
        //               letras que contam 5
      case 'k':
        dados[i + linha_inicial][coluna_inicial + 3][2] = '5';
        break;
        //               letras que contam 8
      case 'j':
      case 'x':
        dados[i + linha_inicial][coluna_inicial + 3][2] = '8';
        break;
        //               letras que contam 10 (usa : porque 10 nao existe em
        //               codigo ascci,e : é o caracter aseguir a 9)
      case 'q':
      case 'z':
        dados[i + linha_inicial][coluna_inicial + 3][2] = ':';
        break;
      default:
        dados[i + linha_inicial][coluna_inicial + 3][2] = '0';
        break;
      }
    }

    for (int i = 0; i < tamanho_palavra; i++) {
      dados[i + linha_inicial][coluna_inicial + 3][3] =
          ((int)(dados[i + linha_inicial][coluna_inicial + 3][1]) - 48) *
          ((int)(dados[i + linha_inicial][coluna_inicial + 3][2]) - 48);
      if (dados[i + linha_inicial][coluna_inicial + 3][1] == '2' ||
          dados[i + linha_inicial][coluna_inicial + 3][1] == '3') {
        dados[i + linha_inicial][coluna_inicial + 3][1] = '1';
      }
    }
  }
}

// pontuacao total
void impressao_pontuacao(int N_jogada, int tamanho_tabuleiro,
                         char jogada_conteudo[], int Linha_jogada,
                         char orientacao_jogada, char Coluna_jogada,
                         int colunas_total, int linhas_total,
                         char dados[linhas_total][colunas_total][7],
                         int *pontuacao_total, int *pontuacao_jogada,
                         int modo_de_jogo) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra,
      pontuacao_da_palavra, multiplicador_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;
  pontuacao_da_palavra = 0;
  multiplicador_palavra = 1;

  if (orientacao_jogada == 'h' || orientacao_jogada == 'H') {
    for (int i = 0; i < tamanho_palavra; i++) {
      pontuacao_da_palavra =
          pontuacao_da_palavra +
          (int)dados[linha_inicial][i + coluna_inicial + 3][3];
    }
    for (int i = 0; i < tamanho_palavra; i++) {
      if ((i + coluna_inicial + 3 == 3 ||
           i + coluna_inicial + 3 == coluna_index) &&
          (linha_inicial == 0 || linha_inicial == linha_index - 2)) {
        multiplicador_palavra = multiplicador_palavra * 2;
      }
    }
  }

  if (orientacao_jogada == 'v' || orientacao_jogada == 'V') {
    for (int i = 0; i < tamanho_palavra; i++) {
      pontuacao_da_palavra =
          pontuacao_da_palavra +
          (int)dados[i + linha_inicial][coluna_inicial + 3][3];
    }
    for (int i = 0; i < tamanho_palavra; i++) {
      if ((coluna_inicial + 3 == 3 || coluna_inicial + 3 == coluna_index) &&
          (i + linha_inicial == 0 || i + linha_inicial == linha_index - 2)) {
        multiplicador_palavra = multiplicador_palavra * 2;
      }
    }
  }
  *pontuacao_jogada = (multiplicador_palavra * pontuacao_da_palavra);
  if (modo_de_jogo == 1) {
    *pontuacao_total =
        *pontuacao_total + (multiplicador_palavra * pontuacao_da_palavra);
  }
}

//                               FUNÇÕES DE ANALISE DO ALGORITMO
//    (Baseadas nas de cima, diferem pouco das originais, pois apenas guardam os
//    dados de forma provisoria)

// Validacao da possivel jogada
int validacao_possivel_da_jogada(int N_jogada, int tamanho_tabuleiro,
                                 char jogada_conteudo[], int Linha_jogada,
                                 char orientacao_jogada, char Coluna_jogada,
                                 int colunas_total, int linhas_total,
                                 char dados[linhas_total][colunas_total][7],
                                 char erros[6]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra,
      validade = 0, validade_dos_caracteres = 0, validade_das_posicoes = 0,
      igualdade_caracteres = 0;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;
  //    Se é horizontal:

  if (orientacao_jogada == 'h' || orientacao_jogada == 'H') {

    //        TESTE DOS DIFERENTES CRITERIOS:
    if ((coluna_inicial <= 3) && (coluna_inicial >= coluna_index) &&
        (linha_inicial <= 0) && (linha_inicial >= linha_index - 2)) {
      return 0;
    }

    if (N_jogada == 1) {
      // A primeira jogada esta na casa central
      if (coluna_inicial != 3 + ((tamanho_tabuleiro - 1) / 2) &&
          linha_inicial != ((tamanho_tabuleiro - 1) / 2)) {
        return 0;
      } else {
        erros[0] = '1';
      }
      // 1 : Cabe no tabuleiro
      if ((Coluna_jogada + tamanho_palavra - 1 <= 'A' + tamanho_tabuleiro) ||
          (Coluna_jogada + tamanho_palavra - 1 <= 'a' + tamanho_tabuleiro)) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 2 : validade dos caracteres (ter cuidado com onde comeca o i!)

      for (int i = 0; i < tamanho_palavra; i++) {
        int c1 = (int)jogada_conteudo[i];
        if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) {
          validade_dos_caracteres++;
        }
      }
      if (validade_dos_caracteres == tamanho_palavra) {
        validade++;
      } else {
        erros[2] = '1';
      }
      // 3 : Nao ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[linha_inicial][i + coluna_inicial + 3][0] != '#') {
          validade_das_posicoes++;
        }
      }
      if (validade_das_posicoes == tamanho_palavra) {
        validade++;
      } else {
        erros[3] = '1';
      }
      // 4ª comprimento
      if (tamanho_palavra >= 2 && tamanho_palavra <= tamanho_tabuleiro) {
        validade++;
      } else {
        erros[4] = '1';
      }

      if (validade == 4) {
        return 1;
      } else {
        return 0;
      }
    }

    else {
      if ((coluna_inicial <= 3) && (coluna_inicial >= coluna_index) &&
          (linha_inicial <= 0) && (linha_inicial >= linha_index - 2)) {
        return 0;
      }

      // 1 : Tem pelo menos um caracter igual;
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[linha_inicial][i + coluna_inicial + 3][0] ==
            jogada_conteudo[i]) {
          igualdade_caracteres++;
        }
      }
      for (int i = 0; i < tamanho_palavra; i++) {
        int c1 = (int)dados[linha_inicial][i + coluna_inicial + 3][0];
        if (((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) &&
            (dados[linha_inicial][i + coluna_inicial + 3][0] !=
             jogada_conteudo[i])) {
          return 0;
        }
      }
      if (igualdade_caracteres > 0) {
        validade++;
      } else {
        erros[5] = '1';
      }
      // 2 : Cabe no tabuleiro
      if (Coluna_jogada + tamanho_palavra - 1 <= 'A' + coluna_index - 3 ||
          Coluna_jogada + tamanho_palavra - 1 <= 'a' + coluna_index - 3) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 3 : validade dos caracteres (ter cuidado com onde comeca o i!)

      for (int i = 0; i < tamanho_palavra; i++) {
        int c1 = (int)jogada_conteudo[i];
        if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) {
          validade_dos_caracteres++;
        }
      }
      if (validade_dos_caracteres == tamanho_palavra) {
        validade++;
      } else {
        erros[2] = '1';
      }
      // 4 : Nao ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[linha_inicial][i + coluna_inicial + 3][0] != '#') {
          validade_das_posicoes++;
        }
      }
      if (validade_das_posicoes == tamanho_palavra) {
        validade++;
      } else {
        erros[3] = '1';
      }
      // 5ª: comprimento
      if ((tamanho_palavra >= 2) && (tamanho_palavra <= tamanho_tabuleiro)) {
        validade++;
      } else {
        erros[4] = '1';
      }

      if (validade == 5) {
        return 1;
      } else {
        return 0;
      }
    }
  }
  // Se é vertical
  else if (orientacao_jogada == 'v' || orientacao_jogada == 'V') {

    if (N_jogada == 1) {
      if ((coluna_inicial <= 3) && (coluna_inicial >= coluna_index) &&
          (linha_inicial <= 0) && (linha_inicial >= linha_index - 2)) {
        return 0;
      }

      if (coluna_inicial != 3 + ((tamanho_tabuleiro - 1) / 2) &&
          linha_inicial != ((tamanho_tabuleiro - 1) / 2)) {
        return 0;
      } else {
        erros[0] = '1';
      }
      // 1 : Cabe no tabuleiro
      if (linha_inicial + tamanho_palavra - 1 <= linha_index) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 2 : validade dos caracteres (ter cuidado com onde comeca o i!)

      for (int i = 0; i < tamanho_palavra; i++) {
        int c1 = (int)jogada_conteudo[i];
        if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) {
          validade_dos_caracteres++;
        }
      }
      if (validade_dos_caracteres == tamanho_palavra) {
        validade++;
      } else {
        erros[2] = '1';
      }
      // 3 : Nao ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[i + linha_inicial][coluna_inicial + 3][0] != '#') {
          validade_das_posicoes++;
        }
      }
      if (validade_das_posicoes == tamanho_palavra) {
        validade++;
      } else {
        erros[3] = '1';
      }
      //        4ª comprimento
      if (tamanho_palavra >= 2 && tamanho_palavra <= tamanho_tabuleiro) {
        validade++;
      } else {
        erros[4] = '1';
      }

      if (validade == 4) {
        return 1;
      } else {
        return 0;
      }
    }

    else {
      // 1 : Tem pelo menos um caracter igual;
      if ((coluna_inicial <= 3) && (coluna_inicial >= coluna_index) &&
          (linha_inicial <= 0) && (linha_inicial >= linha_index - 2)) {
        return 0;
      }

      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[i + linha_inicial][coluna_inicial + 3][0] ==
            jogada_conteudo[i]) {
          igualdade_caracteres++;
        }
      }
      if (igualdade_caracteres > 0) {
        validade++;
      } else {
        erros[5] = '1';
      }
      for (int i = 0; i < tamanho_palavra; i++) {
        int c1 = (int)dados[linha_inicial + i][coluna_inicial + 3][0];
        if (((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) &&
            (dados[i + linha_inicial][coluna_inicial + 3][0] !=
             jogada_conteudo[i])) {
          return 0;
        }
      }
      // 2 : Cabe no tabuleiro
      if (linha_inicial + tamanho_palavra - 1 <= linha_index) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 3 : validade dos caracteres (ter cuidado com onde comeca o i!)

      for (int i = 0; i < tamanho_palavra; i++) {
        int c1 = (int)jogada_conteudo[i];
        if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) {
          validade_dos_caracteres++;
        }
      }
      if (validade_dos_caracteres == tamanho_palavra) {
        validade++;
      } else {
        erros[2] = '1';
      }
      // 4 : Nao ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[i + linha_inicial][coluna_inicial + 3][0] != '#') {
          validade_das_posicoes++;
        }
      }
      if (validade_das_posicoes == tamanho_palavra) {
        validade++;
      } else {
        erros[3] = '1';
      }
      // 5ª comprimento
      if (tamanho_palavra >= 2 && tamanho_palavra <= tamanho_tabuleiro) {
        validade++;
      } else {
        erros[4] = '1';
      }

      if (validade == 5) {
        return 1;
      } else {
        return 0;
      }
    }
  } else {
    return 0;
  }
}

// Contabilizacao de Pontos
// Tem apenas em contabilizacao a palavra em si e os bonus por letra, nao inclui
// os bonus de palavra
void contabilizacao_possivel_pontos(
    int N_jogada, int tamanho_tabuleiro, char jogada_conteudo[],
    int Linha_jogada, char orientacao_jogada, char Coluna_jogada,
    int colunas_total, int linhas_total,
    char dados[linhas_total][colunas_total][7]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;

  if (orientacao_jogada == 'h' || orientacao_jogada == 'H') {
    for (int i = 0; i < tamanho_palavra; i++) {
      switch (tolower(jogada_conteudo[i])) {
        //               letras que contam 1
      case 'a':
      case 'e':
      case 'i':
      case 'l':
      case 'o':
      case 'r':
      case 's':
      case 't':
      case 'u':
        dados[linha_inicial][i + coluna_inicial + 3][5] = '1';
        break;
        //               letras que contam 2
      case 'd':
      case 'g':
        dados[linha_inicial][i + coluna_inicial + 3][5] = '2';
        break;
        //               letras que contam 3
      case 'b':
      case 'c':
      case 'm':
      case 'p':
        dados[linha_inicial][i + coluna_inicial + 3][5] = '3';
        break;
        //               letras que contam 4
      case 'f':
      case 'h':
      case 'v':
      case 'w':
      case 'y':
        dados[linha_inicial][i + coluna_inicial + 3][5] = '4';
        break;
        //               letras que contam 5
      case 'k':
        dados[linha_inicial][i + coluna_inicial + 3][5] = '5';
        break;
        //               letras que contam 8
      case 'j':
      case 'x':
        dados[linha_inicial][i + coluna_inicial + 3][5] = '8';
        break;
        //               letras que contam 10 (usa : porque 10 nao existe em
        //               codigo ascci,e : é o caracter aseguir a 9)
      case 'q':
      case 'z':
        dados[linha_inicial][i + coluna_inicial + 3][5] = ':';
        break;
      default:
        dados[linha_inicial][i + coluna_inicial + 3][5] = '0';
        break;
      }
    }

    for (int i = 0; i < tamanho_palavra; i++) {
      dados[linha_inicial][i + coluna_inicial + 3][6] =
          ((int)(dados[linha_inicial][i + coluna_inicial + 3][1]) - 48) *
          ((int)(dados[linha_inicial][i + coluna_inicial + 3][5]) - 48);
    }
  }

  if (orientacao_jogada == 'v' || orientacao_jogada == 'V') {
    for (int i = 0; i < tamanho_palavra; i++) {
      switch (tolower(jogada_conteudo[i])) {
        //               letras que contam 1
      case 'a':
      case 'e':
      case 'i':
      case 'l':
      case 'o':
      case 'r':
      case 's':
      case 't':
      case 'u':
        dados[i + linha_inicial][coluna_inicial + 3][5] = '1';
        break;
        //               letras que contam 2
      case 'd':
      case 'g':
        dados[i + linha_inicial][coluna_inicial + 3][5] = '2';
        break;
        //               letras que contam 3
      case 'b':
      case 'c':
      case 'm':
      case 'p':
        dados[i + linha_inicial][coluna_inicial + 3][5] = '3';
        break;
        //               letras que contam 4
      case 'f':
      case 'h':
      case 'v':
      case 'w':
      case 'y':
        dados[i + linha_inicial][coluna_inicial + 3][5] = '4';
        break;
        //               letras que contam 5
      case 'k':
        dados[i + linha_inicial][coluna_inicial + 3][5] = '5';
        break;
        //               letras que contam 8
      case 'j':
      case 'x':
        dados[i + linha_inicial][coluna_inicial + 3][5] = '8';
        break;
        //               letras que contam 10 (usa : porque 10 nao existe em
        //               codigo ascci,e : é o caracter aseguir a 9)
      case 'q':
      case 'z':
        dados[i + linha_inicial][coluna_inicial + 3][5] = ':';
        break;
      default:
        dados[i + linha_inicial][coluna_inicial + 3][5] = '0';
        break;
      }
    }

    for (int i = 0; i < tamanho_palavra; i++) {
      dados[i + linha_inicial][coluna_inicial + 3][6] =
          ((int)(dados[i + linha_inicial][coluna_inicial + 3][5]) - 48) *
          ((int)(dados[i + linha_inicial][coluna_inicial + 3][1]) - 48);
    }
  }
}

// pontuacao jogada
void pontuacao_jogada_possivel(int N_jogada, int tamanho_tabuleiro,
                               char jogada_conteudo[], int Linha_jogada,
                               char orientacao_jogada, char Coluna_jogada,
                               int colunas_total, int linhas_total,
                               char dados[linhas_total][colunas_total][7],
                               int *pontuacao_jogada, int modo_de_jogo) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra,
      pontuacao_da_palavra, multiplicador_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;
  pontuacao_da_palavra = 0;
  multiplicador_palavra = 1;
  *pontuacao_jogada = 0;

  if (orientacao_jogada == 'h' || orientacao_jogada == 'H') {
    for (int i = 0; i < tamanho_palavra; i++) {
      pontuacao_da_palavra =
          pontuacao_da_palavra +
          (int)dados[linha_inicial][i + coluna_inicial + 3][6];
    }
    for (int i = 0; i < tamanho_palavra; i++) {
      if ((i + coluna_inicial + 3 == 3 ||
           i + coluna_inicial + 3 == coluna_index) &&
          (linha_inicial == 0 || linha_inicial == linha_index - 2)) {
        multiplicador_palavra = multiplicador_palavra * 2;
      }
    }
  }

  if (orientacao_jogada == 'v' || orientacao_jogada == 'V') {
    for (int i = 0; i < tamanho_palavra; i++) {
      pontuacao_da_palavra =
          pontuacao_da_palavra +
          (int)dados[i + linha_inicial][coluna_inicial + 3][6];
    }
    for (int i = 0; i < tamanho_palavra; i++) {
      if ((coluna_inicial + 3 == 3 || coluna_inicial + 3 == coluna_index) &&
          (i + linha_inicial == 0 || i + linha_inicial == linha_index - 2)) {
        multiplicador_palavra = multiplicador_palavra * 2;
      }
    }
  }
  *pontuacao_jogada = (multiplicador_palavra * pontuacao_da_palavra);
}
// Insere possivel jogada no tabuleiro
void Insere_possivel_jogada_tabuleiro(
    int N_jogada, int tamanho_tabuleiro, char jogada_conteudo[],
    int Linha_jogada, char orientacao_jogada, char Coluna_jogada,
    int colunas_total, int linhas_total,
    char dados[linhas_total][colunas_total][7]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;

  if (orientacao_jogada == 'h' || orientacao_jogada == 'H') {
    for (int i = 0; i < tamanho_palavra; i++) {
      dados[linha_inicial][i + coluna_inicial + 3][4] = jogada_conteudo[i];
    }
  }

  if (orientacao_jogada == 'v' || orientacao_jogada == 'V') {
    for (int i = 0; i < tamanho_palavra; i++) {
      dados[i + linha_inicial][coluna_inicial + 3][4] = jogada_conteudo[i];
    }
  }
}

// Remove posivel jogada no tabuleiro
void remove_posivel_jogada_tabuleiro(
    int N_jogada, int tamanho_tabuleiro, char jogada_conteudo[],
    int Linha_jogada, char orientacao_jogada, char Coluna_jogada,
    int colunas_total, int linhas_total,
    char dados[linhas_total][colunas_total][7]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;

  if (orientacao_jogada == 'h' || orientacao_jogada == 'H') {
    for (int i = 0; i < tamanho_palavra; i++) {
      dados[linha_inicial][i + coluna_inicial + 3][4] = ' ';
      dados[linha_inicial][i + coluna_inicial + 3][5] = '0';
      dados[linha_inicial][i + coluna_inicial + 3][6] = '0';
    }
  }

  if (orientacao_jogada == 'v' || orientacao_jogada == 'V') {
    for (int i = 0; i < tamanho_palavra; i++) {
      dados[i + linha_inicial][coluna_inicial + 3][4] = ' ';
      dados[i + linha_inicial][coluna_inicial + 3][5] = '0';
      dados[i + linha_inicial][coluna_inicial + 3][6] = '0';
    }
  }
}

// fim do jogo
int fim_do_jogo(int tamanho_tabuleiro, char jogada_conteudo[], int Linha_jogada,
                char orientacao_jogada, char Coluna_jogada, int colunas_total,
                int linhas_total, char dados[linhas_total][colunas_total][7]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;
  char fim[] = "fim";
  int igualdade_caracteres = 0;
  for (int i = 0; i < tamanho_palavra; i++) {
    if (fim[i] == tolower(jogada_conteudo[i])) {
      igualdade_caracteres++;
    }
  }
  if (igualdade_caracteres == 3) {
    return 1;
  } else {
    return 0;
  }
}

// leitura do dicionario

const int Tamanho_realocacao = 1;
char **carregar_ficheiro(char *File_dicionario, int *N_Colunas_DIC) {
  char leitura[100];
  FILE *dicionario = fopen(File_dicionario, "r");
  if (!dicionario) {
    printf("Nao foi possivel abrir o ficheiro do dicionario na leitura\n");
  }
  int tamanho_dinamico = Tamanho_realocacao;
  char **linhas = (char **)malloc(tamanho_dinamico * sizeof(char *));
  int i = 0;
  while (fgets(leitura, 100, dicionario)) {
    for (int x = 0; x < 100; x++) {
      leitura[x] = tolower(leitura[x]);
    }
    if (i == tamanho_dinamico) {
      tamanho_dinamico += Tamanho_realocacao;
      char **linhasRE = realloc(linhas, tamanho_dinamico * sizeof(char *));
      if (!linhasRE) {
        printf("Nao foi possivel realocar a memoria");
      }
      linhas = linhasRE;
    }
    for (int a = 0; a < 100; a++) {
      if (leitura[a] == (char)39) {
        leitura[a] = '\0';
        int b;
        for (b = a + 1; b < 100; b++) {
          leitura[b] = '\0';
        }
      }
    }

    leitura[strlen(leitura) - 1] = '\0';
    char *palavra = (char *)malloc((strlen(leitura) + 1) * sizeof(char));
    strcpy(palavra, leitura);
    linhas[i] = palavra;
    i++;
  }
  *N_Colunas_DIC = i;
  return linhas;
}

// Criar array das jogadas possiveis
char **Criar_array_jogadas(int *N_total_jogadas_possiveis) {
  int tamanho_dinamico = Tamanho_realocacao;
  char **linhas_jogadas = (char **)malloc(tamanho_dinamico * sizeof(char *));
  int i = 0;
  *N_total_jogadas_possiveis = i;
  return linhas_jogadas;
}

// Cria string da jogada possivel
char *cria_string(char Coluna_jogada, char orientacao_jogada, int Linha_jogada,
                  int pontuacao_jogada, char *jogada_conteudo) {
  int tamanho_linha = 1;
  if (Linha_jogada >= 10) {
    tamanho_linha = 2;
  }
  int tamanho_pontuacao = 1;
  if (pontuacao_jogada >= 10) {
    tamanho_pontuacao = 2;
  }
  int tamanho_palavra = strlen(jogada_conteudo);
  int tamanho_total = (4 + tamanho_linha + tamanho_palavra + tamanho_pontuacao);
  char *jogada_possivel = (char *)malloc((tamanho_total + 7) * sizeof(char));
  int i = 0;
  int a = (tamanho_linha + 3);
  for (i = 0; i < tamanho_total; i++) {
    if (i == 0) {
      jogada_possivel[i] = Coluna_jogada;
    }
    if ((i == 1) || (i == tamanho_linha)) {
      if (tamanho_linha == 1) {
        jogada_possivel[1] = '1' + (Linha_jogada - 1);
      }
      if (tamanho_linha == 2) {
        jogada_possivel[1] = '1';
        jogada_possivel[2] = '1' + (Linha_jogada - 11);
      }
    }
    if (i == (tamanho_linha + 1)) {
      jogada_possivel[i] = orientacao_jogada;
    }
    if (i == (tamanho_linha + 2)) {
      jogada_possivel[i] = ' ';
    }
    if ((i >= (tamanho_linha + 3)) &&
        (i <= tamanho_linha + 2 + tamanho_palavra)) {
      jogada_possivel[i] = jogada_conteudo[i - a];
    }
    if (i == (tamanho_linha + 3 + tamanho_palavra)) {
      jogada_possivel[i] = ' ';
    }
    if ((i == tamanho_linha + 4 + tamanho_palavra) ||
        (i == tamanho_linha + 5 + tamanho_palavra)) {
      if (tamanho_pontuacao == 1) {
        jogada_possivel[i] = '1' + (Linha_jogada - 1);
      }
      if (tamanho_pontuacao == 2) {
        jogada_possivel[tamanho_total - 2] = '1' + (pontuacao_jogada / 10) - 1;
        jogada_possivel[tamanho_total - 1] =
            '1' + (pontuacao_jogada - 1) - ((pontuacao_jogada / 10) * 10);
      }
    }
  }
  strcat(jogada_possivel, " pontos");
  return jogada_possivel;
}

// Insere a jogada no array
char **Insere_jogada_possivel_array(char **Jogadas_Possiveis,
                                    int *N_total_jogadas_possiveis,
                                    char *jogada_possivel, int N_jogada,
                                    int *N_jogada_array) {
  int i = 0;
  int tamanho_dinamico = Tamanho_realocacao;
  if (i == tamanho_dinamico) {
    tamanho_dinamico += Tamanho_realocacao;
    char **Jogadas_possiveisRE =
        realloc(Jogadas_Possiveis, tamanho_dinamico * sizeof(char *));
    if (!Jogadas_possiveisRE) {
      printf("Nao foi possivel realocar a memoria");
    }
    Jogadas_Possiveis = Jogadas_possiveisRE;
    if (*N_jogada_array != N_jogada) {
      N_jogada_array++;

      int tamanho_N_jogada = 1;
      if (N_jogada > 10) {
        tamanho_N_jogada = 2;
      }

      char *N_AJOGADA = (char *)malloc((tamanho_N_jogada + 8) * sizeof(char));
      for (int a = 0; a < 2; a++) {
        if ((a == 0) || (a == 1)) {
          if (tamanho_N_jogada == 1) {
            N_AJOGADA[0] = '1' + (N_jogada - 1);
          }
          if (tamanho_N_jogada == 2) {
            N_AJOGADA[0] = '1' + (N_jogada / 10) - 1;
            N_AJOGADA[1] = '1' + (N_jogada - 1) - ((N_jogada / 10) * 10);
          }
        }
      }
      strcat(N_AJOGADA, " Jogada:");
      Jogadas_Possiveis[i] = N_AJOGADA;
    } else {
      Jogadas_Possiveis[i] = jogada_possivel;
    }
    i++;
  }
  *N_total_jogadas_possiveis = i;
  return Jogadas_Possiveis;
}

// Criar array das jogadas possiveis
char **Criar_array_casas(int *N_casas_total) {
  int tamanho_dinamico = Tamanho_realocacao;
  char **linhas_casas = (char **)malloc(tamanho_dinamico * sizeof(char *));
  int i = 0;
  *N_casas_total = i;
  return linhas_casas;
}

// Insere a casa no array
char **Insere_casa_possivel_array(char **casas_possiveis, int *N_casas_total,
                                  int coluna, int linha, char orientacao) {
  int i;
  char casa[3];
  casa[0] = (char)coluna;
  casa[1] = (char)linha;
  casa[2] = orientacao;
  printf("%s ", casa);
  i = *N_casas_total;
  int tamanho_dinamico = Tamanho_realocacao;
  if (i == tamanho_dinamico) {
    tamanho_dinamico += Tamanho_realocacao;
    char **casas_possiveisRE =
        realloc(casas_possiveis, tamanho_dinamico * sizeof(char *));
    if (!casas_possiveisRE) {
      printf("Nao foi possivel realocar a memoria");
    }
    casas_possiveis = casas_possiveisRE;
  }
  char *palavra = (char *)malloc(3 * sizeof(char));
  strcpy(palavra, casa);
  casas_possiveis[i] = palavra;
  printf(" %s", casas_possiveis[i]);
  i++;
  *N_casas_total = i;

  return casas_possiveis;
}

//  Marca as casas a analisar
/* Dois modos:Horizontal e vertical
 Horizontal:
 Anda de casa em casa até encontrar uma casa com se a casa nao é proibida
 nem e imendiatamente estiver uma letra em cima ou em baixo a casa é
 passada ao algoritmo
 */
void Marcar_casas(int N_jogada, char *Coluna_jogada, char *orientacao_jogada,
                  int *Linha_jogada, char jogada_conteudo[100],
                  int tamanho_tabuleiro, int pontuacao_jogada,
                  int colunas_total, int linhas_total,
                  char dados[linhas_total][colunas_total][7], int modo_de_jogo,
                  char **casas_possiveis, int *N_casas_total) {

  int linha_inicial, coluna_inicial;
  coluna_inicial = ((int)tolower(*Coluna_jogada)) - 97;
  linha_inicial = (*Linha_jogada) - 1;

  // horizontal

  (pontuacao_jogada) = 0;
  (*Coluna_jogada) = 'A';
  (*orientacao_jogada) = 'h';
  (*Linha_jogada) = 1;
  coluna_inicial = ((int)tolower(*Coluna_jogada)) - 97;
  linha_inicial = (*Linha_jogada) - 1;

  if (N_jogada == 1) {
    int linha1 = 'a' + ((tamanho_tabuleiro - 1) / 2);
    int coluna1 = 'a' + ((tamanho_tabuleiro - 1) / 2);
    casas_possiveis = Insere_casa_possivel_array(
        casas_possiveis, N_casas_total, coluna1, linha1, *orientacao_jogada);
  } else {
    for (int linha = 0; linha < tamanho_tabuleiro; linha++) {
      for (int coluna = 0; coluna < tamanho_tabuleiro; coluna++) {
        int c1 = dados[linha_inicial + linha][coluna + coluna_inicial + 3][0];
        if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) {
          int linha1 = linha_inicial + linha;
          int coluna1 = coluna + coluna_inicial;
          casas_possiveis =
              Insere_casa_possivel_array(casas_possiveis, N_casas_total,
                                         coluna1, linha1, *orientacao_jogada);
        }
      }
    }
  }

  // vertical
  (pontuacao_jogada) = 0;
  (*Coluna_jogada) = 'A';
  (*orientacao_jogada) = 'v';
  (*Linha_jogada) = 1;
  coluna_inicial = ((int)tolower(*Coluna_jogada)) - 97;
  linha_inicial = (*Linha_jogada) - 1;

  if (N_jogada == 1) {
    int linha1 = 'a' + ((tamanho_tabuleiro - 1) / 2);
    int coluna1 = 'a' + ((tamanho_tabuleiro - 1) / 2);
    casas_possiveis = Insere_casa_possivel_array(
        casas_possiveis, N_casas_total, coluna1, linha1, *orientacao_jogada);
  } else {
    for (int linha = 0; linha < tamanho_tabuleiro; linha++) {
      for (int coluna = 0; coluna < tamanho_tabuleiro; coluna++) {
        int c1 = dados[linha_inicial + linha][coluna + coluna_inicial + 3][0];
        int c2 = dados[linha_inicial + linha][coluna + coluna_inicial + 2][0];
        int c3 = dados[linha_inicial + linha][coluna + coluna_inicial + 4][0];
        if (((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) ||
            ((c2 >= 65 && c2 <= 90) || (c2 >= 97 && c2 <= 122)) ||
            ((c3 >= 65 && c3 <= 90) || (c3 >= 97 && c3 <= 122))) {
          int linha1 = linha_inicial + linha;
          int coluna1 = coluna + coluna_inicial;
          casas_possiveis =
              Insere_casa_possivel_array(casas_possiveis, N_casas_total,
                                         coluna1, linha1, *orientacao_jogada);
        }
      }
    }
  }

  for (int i = 0; i < *N_casas_total; i++) {
    printf("\n%s\n", casas_possiveis[i]);
  }
}

// Analise das casas
void analise_casas(char **casas_possiveis, int N_casas_total, int N_Colunas_DIC,
                   char **Coluna_DIC, int *N_total_jogadas_possiveis,
                   char **Jogadas_Possiveis, int N_jogada,
                   int tamanho_tabuleiro, char *jogada_conteudo,
                   int *Linha_jogada, char *orientacao_jogada,
                   char *Coluna_jogada, int colunas_total, int linhas_total,
                   char dados[linhas_total][colunas_total][7], char erros[6],
                   int modo_de_jogo, int *N_jogada_array) {
  char casa[4];
  int pontuacao_jogada = 0;
  // melhor jogada
  char Coluna_jogada_Max = 'c';
  char orientacao_jogada_Max = 'h';
  int Linha_jogada_Max = 0;
  int pontuacao_jogada_Max = 0;
  char jogada_conteudo_Max[15] = {0};
  jogada_conteudo = (char *)malloc((16) * sizeof(char));

  for (int x = 0; x < N_Colunas_DIC - 1; x++) {
    memcpy(jogada_conteudo, Coluna_DIC[x], strlen(Coluna_DIC[x]));
    printf("%s\n", jogada_conteudo);

    for (int i = 0; i < N_casas_total + 1; i++) {
      memcpy(casa, casas_possiveis[i], 3);
      printf("%s\n", casas_possiveis[i]);
      *Coluna_jogada = casa[0];
      *Linha_jogada = (int)casa[1] - 96;
      *orientacao_jogada = casa[2];

      if (*orientacao_jogada == 'h' || *orientacao_jogada == 'H') {
        int comprimento_da_palavra = strlen(jogada_conteudo);
        for (int b = 0; b < comprimento_da_palavra; b++) {
          *Coluna_jogada = *Coluna_jogada - b;
          Insere_possivel_jogada_tabuleiro(N_jogada, tamanho_tabuleiro,
                                           jogada_conteudo, *Linha_jogada,
                                           *orientacao_jogada, *Coluna_jogada,
                                           colunas_total, linhas_total, dados);
          if (validacao_possivel_da_jogada(
                  N_jogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                  *orientacao_jogada, *Coluna_jogada, colunas_total,
                  linhas_total, dados, erros) == 1) {
            contabilizacao_de_pontos(N_jogada, tamanho_tabuleiro,
                                     jogada_conteudo, *Linha_jogada,
                                     *orientacao_jogada, *Coluna_jogada,
                                     colunas_total, linhas_total, dados);
            pontuacao_jogada_possivel(
                N_jogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                *orientacao_jogada, *Coluna_jogada, colunas_total, linhas_total,
                dados, &pontuacao_jogada, modo_de_jogo);
            if (pontuacao_jogada >= 0.5 * pontuacao_jogada_Max) {
              char *jogada_possivel =
                  cria_string(*Coluna_jogada, *orientacao_jogada, *Linha_jogada,
                              pontuacao_jogada, jogada_conteudo);
              Jogadas_Possiveis = Insere_jogada_possivel_array(
                  Jogadas_Possiveis, N_total_jogadas_possiveis, jogada_possivel,
                  N_jogada, N_jogada_array);
            }
            if ((pontuacao_jogada) > pontuacao_jogada_Max) {
              pontuacao_jogada_Max = (pontuacao_jogada);
              Coluna_jogada_Max = (*Coluna_jogada);
              orientacao_jogada_Max = (*orientacao_jogada);
              Linha_jogada_Max = (*Linha_jogada);
              memcpy(jogada_conteudo_Max, jogada_conteudo,
                     strlen(jogada_conteudo));
            }
            remove_posivel_jogada_tabuleiro(N_jogada, tamanho_tabuleiro,
                                            jogada_conteudo, *Linha_jogada,
                                            *orientacao_jogada, *Coluna_jogada,
                                            colunas_total, linhas_total, dados);
          }
        }
      }
      printf("%s", jogada_conteudo_Max);

      if (*orientacao_jogada == 'v' || *orientacao_jogada == 'V') {
        int comprimento_da_palavra = strlen(jogada_conteudo);
        for (int b = 0; b < comprimento_da_palavra; b++) {
          *Linha_jogada = *Linha_jogada - b;
          Insere_possivel_jogada_tabuleiro(N_jogada, tamanho_tabuleiro,
                                           jogada_conteudo, *Linha_jogada,
                                           *orientacao_jogada, *Coluna_jogada,
                                           colunas_total, linhas_total, dados);
          if (validacao_possivel_da_jogada(
                  N_jogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                  *orientacao_jogada, *Coluna_jogada, colunas_total,
                  linhas_total, dados, erros) == 1) {
            contabilizacao_de_pontos(N_jogada, tamanho_tabuleiro,
                                     jogada_conteudo, *Linha_jogada,
                                     *orientacao_jogada, *Coluna_jogada,
                                     colunas_total, linhas_total, dados);
            pontuacao_jogada_possivel(
                N_jogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                *orientacao_jogada, *Coluna_jogada, colunas_total, linhas_total,
                dados, &pontuacao_jogada, modo_de_jogo);
            if (pontuacao_jogada >= 0.5 * pontuacao_jogada_Max) {
              char *jogada_possivel =
                  cria_string(*Coluna_jogada, *orientacao_jogada, *Linha_jogada,
                              pontuacao_jogada, jogada_conteudo);
              Jogadas_Possiveis = Insere_jogada_possivel_array(
                  Jogadas_Possiveis, N_total_jogadas_possiveis, jogada_possivel,
                  N_jogada, N_jogada_array);
            }
            if ((pontuacao_jogada) > pontuacao_jogada_Max) {
              pontuacao_jogada_Max = (pontuacao_jogada);
              Coluna_jogada_Max = (*Coluna_jogada);
              orientacao_jogada_Max = (*orientacao_jogada);
              Linha_jogada_Max = (*Linha_jogada);
              memcpy(jogada_conteudo_Max, jogada_conteudo,
                     strlen(jogada_conteudo));
            }
          }
          remove_posivel_jogada_tabuleiro(N_jogada, tamanho_tabuleiro,
                                          jogada_conteudo, *Linha_jogada,
                                          *orientacao_jogada, *Coluna_jogada,
                                          colunas_total, linhas_total, dados);
        }
      }
    }
  }

(*Coluna_jogada) = Coluna_jogada_Max;
(*orientacao_jogada) = orientacao_jogada_Max;
(*Linha_jogada) = Linha_jogada_Max;
memcpy(jogada_conteudo, jogada_conteudo_Max, strlen(jogada_conteudo_Max));
}
//  MAIN
int main(int argc, char *argv[]) {

      int tamanho_tabuleiro = 9, I = 0, colunas_total = 0, linhas_total = 0,
      linha_index_tabuleiro, coluna_index_tabuleiro, gameover = 0, N_jogada = 1;
      int modo_de_jogo = 2;
      int jogadamax;
      int letrasrest;
      char *File_letras="letras.txt";
      char *File_dicionario="/home/public/Downloads/words.txt";
      char *File_tabprev;
      char *File_tabfinal;
      char *File_registo;


    int opt= 'h';    /* opção para getopt() */

    /* processar as opções da linha de comando */

    /*Print arguments*/
    for( int i = 0; i < argc; ++i ) {
        printf( "argv[ %d ] = %s\n", i, argv[ i ] );
    }

    /*debuggerprintf("dsds");*/
    while ((opt= getopt(argc, argv, "h:t:d:l:m:n:i:j:o:r")) != -1) {
        switch (opt) {
            case 't':
               /*validar*/  /* se houver erro, fica o valor por omissão */
                tamanho_tabuleiro=optarg[0];
                break;
            case 'd':
                strcpy( File_dicionario,optarg);
                break;
            case 'l':
                strcpy(File_letras,optarg );
                break;
            case 'm':
                sscanf(optarg, "%d", &letrasrest);
                break;
            case 'n':
                sscanf(optarg, "%d", &jogadamax);
                break;
            case 'i':
                strcpy(File_tabprev,optarg );
                break;
            case 'j':
                sscanf(optarg, "%d", &modo_de_jogo);
                break;
            case 'o':
                strcpy(File_tabfinal,optarg );
                break;
            case 'r':
                 strcpy(File_registo,optarg );
                break;
            case 'h': /* help */
                help(argv[0]);
                break;
            default: /* '?' opções inválidas */
                break;
        }
    }


  /*while (I == 0) {
    printf("Insira o tamanho do tabuleiro:\n");
    scanf("%d", &tamanho_tabuleiro);
    if (tamanho_tabuleiro % 2 != 0 && tamanho_tabuleiro <= 15 &&
        tamanho_tabuleiro >= 7) {
      I = 1;
    }
  }

  printf("Insira o modo de jogo:\n");
  scanf("%d", &modo_de_jogo);
  */
  int Pontuacao_total = 0;
  linhas_total = tamanho_tabuleiro + 2;
  colunas_total = tamanho_tabuleiro + 3;
  char dados[linhas_total][colunas_total][7];
  char erros[6] = {0};
  pre_formatacao(colunas_total, linhas_total, tamanho_tabuleiro, dados);
  gerar_tabuleiro(colunas_total, linhas_total, tamanho_tabuleiro, dados);
  imprimir_tabuleiro(colunas_total, linhas_total, dados);

  // modo de jogo 1
  if (modo_de_jogo == 1) {
    while (gameover == 0) {
      // jogadas variaveis
      int pontuacao_jogada = 0;
      char Coluna_jogada, orientacao_jogada;
      int Linha_jogada;
      char jogada_conteudo[15] = {0};
      char jogada_coordenadas[4] = {0};
      printf("Insira a sua %d jogada:\n", N_jogada);
      scanf("%s %s", jogada_coordenadas, jogada_conteudo);
      Coluna_jogada = jogada_coordenadas[0];
      if (((int)jogada_coordenadas[2] >= 48) &&
          ((int)jogada_coordenadas[2] <= 57)) {
        Linha_jogada =
            (jogada_coordenadas[1] - 48) * 10 + jogada_coordenadas[2] - 48;
        orientacao_jogada = jogada_coordenadas[3];
      } else {
        Linha_jogada = jogada_coordenadas[1] - 48;
        orientacao_jogada = jogada_coordenadas[2];
      }
      if (fim_do_jogo(tamanho_tabuleiro, jogada_conteudo, Linha_jogada,
                      orientacao_jogada, Coluna_jogada, colunas_total,
                      linhas_total, dados) == 1) {
        printf("A sua pontucao total: %d pontos\n", Pontuacao_total);
        gameover = 1;
        return 0;
      }
      if (validacao_da_jogada(N_jogada, tamanho_tabuleiro, jogada_conteudo,
                              Linha_jogada, orientacao_jogada, Coluna_jogada,
                              colunas_total, linhas_total, dados, erros) == 1) {
        Insere_jogada_tabuleiro(N_jogada, tamanho_tabuleiro, jogada_conteudo,
                                Linha_jogada, orientacao_jogada, Coluna_jogada,
                                colunas_total, linhas_total, dados);
        printf("\n%d Jogada:\n", N_jogada);
        imprimir_tabuleiro(colunas_total, linhas_total, dados);
        contabilizacao_de_pontos(N_jogada, tamanho_tabuleiro, jogada_conteudo,
                                 Linha_jogada, orientacao_jogada, Coluna_jogada,
                                 colunas_total, linhas_total, dados);
        printf("\n%s %s", jogada_coordenadas, jogada_conteudo);
        impressao_pontuacao(N_jogada, tamanho_tabuleiro, jogada_conteudo,
                            Linha_jogada, orientacao_jogada, Coluna_jogada,
                            colunas_total, linhas_total, dados,
                            &Pontuacao_total, &pontuacao_jogada, modo_de_jogo);
        printf(" %d Pontos    |", pontuacao_jogada);
        printf("    Pontuacao total: %d\n", Pontuacao_total);
        N_jogada++;
      } else {
        printf("Invalido:\n");
        if (erros[0] == '1') {
          printf(" A primeira jogada deve ser jogada na casa central e na "
                 "horizontal\n");
          erros[0] = '0';
        }
        if (erros[1] == '1') {
          printf("A jogada nao cabe no tabuleiro\n");
          erros[1] = '0';
        }
        if (erros[2] == '1') {
          printf("A palavra contem caracteres invalidos\n");
          erros[2] = '0';
        }
        if (erros[3] == '1') {
          printf("A palavra ocupa casas proibidas\n");
          erros[3] = '0';
        }
        if (erros[4] == '1') {
          printf("O tamanho da palavra ou é menor que 2 ou maior que %d\n",
                 tamanho_tabuleiro);
          erros[4] = '0';
        }
        if (erros[5] == '1') {
          printf("A palavra nao tem pelo menos um caracter igual\n");
          erros[5] = '0';
        }
      }
    }
  }

  // modo de jogo 2
  if (modo_de_jogo == 2) {
    int N_jogada_array = 0;
    // leitura do dicionario
    int N_Colunas_DIC = 0;
    char **Coluna_DIC = carregar_ficheiro(File_dicionario, &N_Colunas_DIC);

    int N_total_jogadas_possiveis = 0;
    int N_casas_total = 0;
    char **casas_possiveis = Criar_array_casas(&N_casas_total);
    char **Jogadas_Possiveis = Criar_array_jogadas(&N_total_jogadas_possiveis);
    while (gameover == 0) {
      int pontuacao_jogada = 0;
      char Coluna_jogada = '0', orientacao_jogada = 'h';
      int Linha_jogada = 0;
      char *jogada_conteudo;
      Marcar_casas(N_jogada, &Coluna_jogada, &orientacao_jogada, &Linha_jogada,
                   jogada_conteudo, tamanho_tabuleiro, pontuacao_jogada,
                   colunas_total, linhas_total, dados, modo_de_jogo,
                   casas_possiveis, &N_casas_total);
      for (int i = 0; i < N_casas_total; i++) {
        printf("\n%s\n", casas_possiveis[i]);
      }
char casa[4];

  // melhor jogada
  char Coluna_jogada_Max = 'c';
  char orientacao_jogada_Max = 'h';
  int Linha_jogada_Max = 0;
  int pontuacao_jogada_Max = 0;
  char jogada_conteudo_Max[15] = {0};
  jogada_conteudo = (char *)malloc((16) * sizeof(char));

  for (int x = 0; x < N_Colunas_DIC - 1; x++) {

    for (int i = 0; i < N_casas_total + 1; i++) {
      memcpy(casa, casas_possiveis[i], 3);
      printf("%s\n", casas_possiveis[i]);
      Coluna_jogada = casa[0];
      Linha_jogada = (int)casa[1] - 96;
      orientacao_jogada = casa[2];

      if (orientacao_jogada == 'h' || orientacao_jogada == 'H') {
        int comprimento_da_palavra = strlen(Coluna_DIC[x]);
        for (int b = 0; b < comprimento_da_palavra; b++) {
          Coluna_jogada = Coluna_jogada - b;
          Insere_possivel_jogada_tabuleiro(N_jogada, tamanho_tabuleiro,
                                           Coluna_DIC[x], Linha_jogada,
                                           orientacao_jogada, Coluna_jogada,
                                           colunas_total, linhas_total, dados);
          if (validacao_possivel_da_jogada(
                  N_jogada, tamanho_tabuleiro, Coluna_DIC[x], Linha_jogada,
                  orientacao_jogada, Coluna_jogada, colunas_total,
                  linhas_total, dados, erros) == 1) {
            contabilizacao_de_pontos(N_jogada, tamanho_tabuleiro,
                                     Coluna_DIC[x], Linha_jogada,
                                     orientacao_jogada, Coluna_jogada,
                                     colunas_total, linhas_total, dados);
            pontuacao_jogada_possivel(
                N_jogada, tamanho_tabuleiro,Coluna_DIC[x], Linha_jogada,
                orientacao_jogada, Coluna_jogada, colunas_total, linhas_total,
                dados, &pontuacao_jogada, modo_de_jogo);
            if (pontuacao_jogada >= 0.5 * pontuacao_jogada_Max) {
              char *jogada_possivel =
                  cria_string(Coluna_jogada, orientacao_jogada, Linha_jogada,
                              pontuacao_jogada, Coluna_DIC[x]);
              Jogadas_Possiveis = Insere_jogada_possivel_array(
                  Jogadas_Possiveis, &N_total_jogadas_possiveis, jogada_possivel,
                  N_jogada, &N_jogada_array);
            }
            if ((pontuacao_jogada) > pontuacao_jogada_Max) {
              pontuacao_jogada_Max = (pontuacao_jogada);
              Coluna_jogada_Max = (Coluna_jogada);
              orientacao_jogada_Max = (orientacao_jogada);
              Linha_jogada_Max = (Linha_jogada);
              memcpy(jogada_conteudo_Max, Coluna_DIC[x],
                     strlen(Coluna_DIC[x]));
            }
            remove_posivel_jogada_tabuleiro(N_jogada, tamanho_tabuleiro,
                                           Coluna_DIC[x], Linha_jogada,
                                            orientacao_jogada, Coluna_jogada,
                                            colunas_total, linhas_total, dados);
          }
        }
      }
      printf("%s", jogada_conteudo_Max);
      printf("1111");

      if (orientacao_jogada == 'v' || orientacao_jogada == 'V') {
        int comprimento_da_palavra = strlen(Coluna_DIC[x]);
        for (int b = 0; b < comprimento_da_palavra; b++) {
          Linha_jogada = Linha_jogada - b;
          Insere_possivel_jogada_tabuleiro(N_jogada, tamanho_tabuleiro,
                                           Coluna_DIC[x], Linha_jogada,
                                           orientacao_jogada, Coluna_jogada,
                                           colunas_total, linhas_total, dados);
          if (validacao_possivel_da_jogada(
                  N_jogada, tamanho_tabuleiro, Coluna_DIC[x], Linha_jogada,
                  orientacao_jogada, Coluna_jogada, colunas_total,
                  linhas_total, dados, erros) == 1) {
            contabilizacao_de_pontos(N_jogada, tamanho_tabuleiro,
                                     Coluna_DIC[x], Linha_jogada,
                                     orientacao_jogada, Coluna_jogada,
                                     colunas_total, linhas_total, dados);
            pontuacao_jogada_possivel(
                N_jogada, tamanho_tabuleiro, Coluna_DIC[x], Linha_jogada,
                orientacao_jogada, Coluna_jogada, colunas_total, linhas_total,
                dados, &pontuacao_jogada, modo_de_jogo);
            if (pontuacao_jogada >= 0.5 * pontuacao_jogada_Max) {
              char *jogada_possivel =
                  cria_string(Coluna_jogada, orientacao_jogada, Linha_jogada,
                              pontuacao_jogada, Coluna_DIC[x]);
              Jogadas_Possiveis = Insere_jogada_possivel_array(
                  Jogadas_Possiveis, &N_total_jogadas_possiveis, jogada_possivel,
                  N_jogada, &N_jogada_array);
            }
            if ((pontuacao_jogada) > pontuacao_jogada_Max) {
              pontuacao_jogada_Max = (pontuacao_jogada);
              Coluna_jogada_Max = (Coluna_jogada);
              orientacao_jogada_Max = (orientacao_jogada);
              Linha_jogada_Max = (Linha_jogada);
              memcpy(jogada_conteudo_Max, Coluna_DIC[x],
                     strlen(jogada_conteudo));
            }
          }
          remove_posivel_jogada_tabuleiro(N_jogada, tamanho_tabuleiro,
                                          Coluna_DIC[x], Linha_jogada,
                                          orientacao_jogada, Coluna_jogada,
                                          colunas_total, linhas_total, dados);
        }
      }
    }
  }
printf("2222");
(Coluna_jogada) = Coluna_jogada_Max;
(orientacao_jogada) = orientacao_jogada_Max;
(Linha_jogada) = Linha_jogada_Max;
memcpy(jogada_conteudo, jogada_conteudo_Max, strlen(jogada_conteudo_Max));

      /*analise_casas(casaspossiveis, N_casas_total, N_Colunas_DIC, Coluna_DIC,
                    &N_total_jogadas_possiveis, Jogadas_Possiveis, N_jogada,
                    tamanho_tabuleiro, jogada_conteudo, &Linha_jogada,
                    &orientacao_jogada, &Coluna_jogada, colunas_total,
                    linhas_total, dados, erros, modo_de_jogo, &N_jogada_array);*/
      Insere_jogada_tabuleiro(N_jogada, tamanho_tabuleiro, jogada_conteudo,
                              Linha_jogada, orientacao_jogada, Coluna_jogada,
                              colunas_total, linhas_total, dados);
      printf("\n%d Jogada:\n", N_jogada);
      imprimir_tabuleiro(colunas_total, linhas_total, dados);
      contabilizacao_de_pontos(N_jogada, tamanho_tabuleiro, jogada_conteudo,
                               Linha_jogada, orientacao_jogada, Coluna_jogada,
                               colunas_total, linhas_total, dados);
      printf("\n%c%d%c %s", Coluna_jogada, Linha_jogada, orientacao_jogada,
             jogada_conteudo);
      impressao_pontuacao(N_jogada, tamanho_tabuleiro, jogada_conteudo,
                          Linha_jogada, orientacao_jogada, Coluna_jogada,
                          colunas_total, linhas_total, dados, &Pontuacao_total,
                          &pontuacao_jogada, modo_de_jogo);
      printf(" %d Pontos    |", pontuacao_jogada);
      printf("    Pontuacao total: %d\n", Pontuacao_total);
      N_jogada++;
      gameover = 0;
      if (N_jogada==jogadamax) {
        gameover = 1;
      }
    }
  }
  if (modo_de_jogo == 3) {
    printf("%s",File_dicionario);
    int N_Colunas_DIC = 0;
    char **Coluna_DIC = carregar_ficheiro(File_dicionario, &N_Colunas_DIC);
 for(int i=0;i<N_Colunas_DIC;i++){

        printf("%s\n",Coluna_DIC[i]);
    }
    printf("%s",Coluna_DIC[5]);
    printf("%d",strlen(Coluna_DIC[5]));
    char *jogada_conteudo;


    }
  }


/* RASCUNHO:
 criar array multidimensional que tem os dados todos do jogo, o que
corresponde cada linha da matrix:
1 dimensao:index de linha no array;
2ªdimensao:index da coluna no array; (estas duas linhas tem as coordenadas da
posicao no tabuleiro)

 3 dimensao(niveis):
 0:Conteudo da posicao no tabuleiro;
 1:Bonus da posicao;
 2:Pontos da letra;
 3:pontos da posicao (1x2:bonus da posicao * pontos da letra);
 4:jogada possivel a ser estudada pelo algoritmo;
 5:Pontos da letra;
 6:pontos da posicao

 ERROS CODIGOS: ARRAY []
 1:A primeira jogada nao cabe no tabuleiro;
 2:cabe no tabuleiro
 3:validade dos caracteres
 4:ocupa casas proibidas 5:a palavra é maior ou menor do que devia
*

 // argumentos da linha comando
     char arg1[2]={0};
     char arg2[2]={0};
     char arg3[2]={0};
     char arg4[2]={0};
     char arg5[2]={0};
     char arg6[2]={0};
     char arg7[2]={0};
     char arg8[2]={0};
     char arg9[2]={0};
     char arg10[2]={0};
 // variaveis relativas aos argumentos da linha de comando
     int ajuda=0;
     char modo_de_jogo;
     int tamanho_tabuleiro;
     char ficheiro_dicionario[]={0};
     char ficheiro_letras[]={0};
     int maximo_letras;
     int maximo_jogadas;
     char ficheiro_carregar_tabuleiro[]={0};
     char ficheiro_guardar_tabuleiro[]={0};
     char ficheiro_jogadas_possiveis[]={0};

 // Procura de argumentos na linha de comandos
             for(int i1=0;i1<=argc;i1++){
                 switch (i1) {
                     case 1:
                         strcpy(arg1,argv[i1]);
                         break;
                     case 3:
                         strcpy(arg2,argv[i1]);
                         break;
                     case 5:
                         strcpy(arg3,argv[i1]);
                         break;
                     case 7:
                         strcpy(arg4,argv[i1]);
                         break;
                     case 9:
                         strcpy(arg5,argv[i1]);
                         break;
                     case 11:
                         strcpy(arg6,argv[i1]);
                         break;
                     case 13:
                         strcpy(arg7,argv[i1]);
                         break;
                     case 15:
                         strcpy(arg8,argv[i1]);
                         break;
                     case 17:
                         strcpy(arg9,argv[i1]);
                         break;
                     case 19:
                         strcpy(arg10,argv[i1]);
                         break;}}

      -h
      -t lxc
      -d filename -l filename -m 5-20
      -N_nn
      -i filename -j 1-4
      -o filename - r filename
      ajuda para o utilizador
      dimensões do tabuleiro (linha x coluna).
      nome do ficheiro de dicionario a utilizar
      nome do ficheiro com letras a usar nas jogadas
      número de letras que um jogador pode ter na sua mao para jogar
      número de jogadas maximo a realizar
      define ficheiro com o tabuleiro a usar em alternativa a jogar num
 tabuleiro vazio modo de jogo 1 a 4 define ficheiro onde escrever o
 tabuleiro final. define ficheiro para registo de todas as jogadas
 possíveis. switch (arg1[1]) { case 'h': ajuda=1; break; case 't':
 tamanho_tabuleiro= break; default: break;
     }

 // impresao do dicionario
for (int i = 0; i < N_Colunas_DIC; i++) {
 printf("%s\n", Coluna_DIC[i]);

 */
