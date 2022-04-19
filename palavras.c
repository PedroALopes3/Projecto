//
//  palavras.c
//
//
//  Created by Pedro Lopes on 15/04/2022.
//

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Pre-formatação do tabuleiro
void pre_formataçao(int colunas_total, int linhas_total, int tamanho_tabuleiro,
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

// Validação da jogada
int validação_da_jogada(int Nºjogada, int tamanho_tabuleiro,
                        char jogada_conteudo[], int Linha_jogada,
                        char orientação_jogada, char Coluna_jogada,
                        int colunas_total, int linhas_total,
                        char dados[linhas_total][colunas_total][7],
                        char erros[6]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra,
      validade = 0, validade_dos_caracteres = 0, validade_das_posiçoes = 0,
      igualdade_caracteres = 0;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;
  //    Se é horizontal:

  if (orientação_jogada == 'h' || orientação_jogada == 'H') {

    //        TESTE DOS DIFERENTES CRITERIOS:

    if (Nºjogada == 1) {
      // A primeira jogada está na casa central
      if (coluna_inicial != 3 + ((tamanho_tabuleiro - 1) / 2) &&
          linha_inicial != ((tamanho_tabuleiro - 1) / 2)) {
        return 0;
      } else {
        erros[0] = '1';
      }
      // 1º: Cabe no tabuleiro
      if ((Coluna_jogada + tamanho_palavra - 1 <= 'A' + tamanho_tabuleiro) ||
          (Coluna_jogada + tamanho_palavra - 1 <= 'a' + tamanho_tabuleiro)) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 2º: validade dos caracteres (ter cuidado com onde começa o i!)

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
      // 3º: Não ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[linha_inicial][i + coluna_inicial + 3][0] != '#') {
          validade_das_posiçoes++;
        }
      }
      if (validade_das_posiçoes == tamanho_palavra) {
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
      // 1º: Tem pelo menos um caracter igual;
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
      // 2º: Cabe no tabuleiro
      if (Coluna_jogada + tamanho_palavra - 1 <= 'A' + coluna_index - 3 ||
          Coluna_jogada + tamanho_palavra - 1 <= 'a' + coluna_index - 3) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 3º: validade dos caracteres (ter cuidado com onde começa o i!)

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
      // 4º: Não ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[linha_inicial][i + coluna_inicial + 3][0] != '#') {
          validade_das_posiçoes++;
        }
      }
      if (validade_das_posiçoes == tamanho_palavra) {
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
  else if (orientação_jogada == 'v' || orientação_jogada == 'V') {

    if (Nºjogada == 1) {
      if (coluna_inicial != 3 + ((tamanho_tabuleiro - 1) / 2) &&
          linha_inicial != ((tamanho_tabuleiro - 1) / 2)) {
        return 0;
      } else {
        erros[0] = '1';
      }
      // 1º: Cabe no tabuleiro
      if (linha_inicial + tamanho_palavra - 1 <= linha_index) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 2º: validade dos caracteres (ter cuidado com onde começa o i!)

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
      // 3º: Não ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[i + linha_inicial][coluna_inicial + 3][0] != '#') {
          validade_das_posiçoes++;
        }
      }
      if (validade_das_posiçoes == tamanho_palavra) {
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
      // 1º: Tem pelo menos um caracter igual;
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
      // 2º: Cabe no tabuleiro
      if (linha_inicial + tamanho_palavra - 1 <= linha_index) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 3º: validade dos caracteres (ter cuidado com onde começa o i!)

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
      // 4º: Não ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[i + linha_inicial][coluna_inicial + 3][0] != '#') {
          validade_das_posiçoes++;
        }
      }
      if (validade_das_posiçoes == tamanho_palavra) {
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
void Insere_jogada_tabuleiro(int Nºjogada, int tamanho_tabuleiro,
                             char jogada_conteudo[], int Linha_jogada,
                             char orientação_jogada, char Coluna_jogada,
                             int colunas_total, int linhas_total,
                             char dados[linhas_total][colunas_total][7]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;

  if (orientação_jogada == 'h' || orientação_jogada == 'H') {
    for (int i = 0; i < tamanho_palavra; i++) {
      dados[linha_inicial][i + coluna_inicial + 3][0] = jogada_conteudo[i];
    }
  }

  if (orientação_jogada == 'v' || orientação_jogada == 'V') {
    for (int i = 0; i < tamanho_palavra; i++) {
      dados[i + linha_inicial][coluna_inicial + 3][0] = jogada_conteudo[i];
    }
  }
}

// Contabilização de Pontos
// Tem apenas em contabilizaçao a palavra em si e os bonus por letra, não inclui
// os bonus de palavra
void contabilização_de_pontos(int Nºjogada, int tamanho_tabuleiro,
                              char jogada_conteudo[], int Linha_jogada,
                              char orientação_jogada, char Coluna_jogada,
                              int colunas_total, int linhas_total,
                              char dados[linhas_total][colunas_total][7]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;

  if (orientação_jogada == 'h' || orientação_jogada == 'H') {
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
        //               letras que contam 10 (usa : porque 10 não existe em
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

  if (orientação_jogada == 'v' || orientação_jogada == 'V') {
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
        //               letras que contam 10 (usa : porque 10 não existe em
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

// pontuação total
void impressão_pontuação(int Nºjogada, int tamanho_tabuleiro,
                         char jogada_conteudo[], int Linha_jogada,
                         char orientação_jogada, char Coluna_jogada,
                         int colunas_total, int linhas_total,
                         char dados[linhas_total][colunas_total][7],
                         int *pontuação_total, int *pontuação_jogada,
                         int modo_de_jogo) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra,
      pontuaçao_da_palavra, multiplicador_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;
  pontuaçao_da_palavra = 0;
  multiplicador_palavra = 1;

  if (orientação_jogada == 'h' || orientação_jogada == 'H') {
    for (int i = 0; i < tamanho_palavra; i++) {
      pontuaçao_da_palavra =
          pontuaçao_da_palavra +
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

  if (orientação_jogada == 'v' || orientação_jogada == 'V') {
    for (int i = 0; i < tamanho_palavra; i++) {
      pontuaçao_da_palavra =
          pontuaçao_da_palavra +
          (int)dados[i + linha_inicial][coluna_inicial + 3][3];
    }
    for (int i = 0; i < tamanho_palavra; i++) {
      if ((coluna_inicial + 3 == 3 || coluna_inicial + 3 == coluna_index) &&
          (i + linha_inicial == 0 || i + linha_inicial == linha_index - 2)) {
        multiplicador_palavra = multiplicador_palavra * 2;
      }
    }
  }
  *pontuação_jogada = (multiplicador_palavra * pontuaçao_da_palavra);
  if (modo_de_jogo == 1) {
    *pontuação_total =
        *pontuação_total + (multiplicador_palavra * pontuaçao_da_palavra);
  }
}

//                               FUNÇÕES DE ANALISE DO ALGORITMO
//    (Baseadas nas de cima, diferem pouco das originais, pois apenas guardam os
//    dados de forma provisoria)

// Validação da possivel jogada
int validação_possivel_da_jogada(int Nºjogada, int tamanho_tabuleiro,
                                 char jogada_conteudo[], int Linha_jogada,
                                 char orientação_jogada, char Coluna_jogada,
                                 int colunas_total, int linhas_total,
                                 char dados[linhas_total][colunas_total][7],
                                 char erros[6]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra,
      validade = 0, validade_dos_caracteres = 0, validade_das_posiçoes = 0,
      igualdade_caracteres = 0;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;
  //    Se é horizontal:

  if (orientação_jogada == 'h' || orientação_jogada == 'H') {

    //        TESTE DOS DIFERENTES CRITERIOS:
    if ((coluna_inicial <= 3) && (coluna_inicial >= coluna_index) &&
        (linha_inicial <= 0) && (linha_inicial >= linha_index - 2)) {
      return 0;
    }

    if (Nºjogada == 1) {
      // A primeira jogada está na casa central
      if (coluna_inicial != 3 + ((tamanho_tabuleiro - 1) / 2) &&
          linha_inicial != ((tamanho_tabuleiro - 1) / 2)) {
        return 0;
      } else {
        erros[0] = '1';
      }
      // 1º: Cabe no tabuleiro
      if ((Coluna_jogada + tamanho_palavra - 1 <= 'A' + tamanho_tabuleiro) ||
          (Coluna_jogada + tamanho_palavra - 1 <= 'a' + tamanho_tabuleiro)) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 2º: validade dos caracteres (ter cuidado com onde começa o i!)

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
      // 3º: Não ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[linha_inicial][i + coluna_inicial + 3][0] != '#') {
          validade_das_posiçoes++;
        }
      }
      if (validade_das_posiçoes == tamanho_palavra) {
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

      // 1º: Tem pelo menos um caracter igual;
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
      // 2º: Cabe no tabuleiro
      if (Coluna_jogada + tamanho_palavra - 1 <= 'A' + coluna_index - 3 ||
          Coluna_jogada + tamanho_palavra - 1 <= 'a' + coluna_index - 3) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 3º: validade dos caracteres (ter cuidado com onde começa o i!)

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
      // 4º: Não ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[linha_inicial][i + coluna_inicial + 3][0] != '#') {
          validade_das_posiçoes++;
        }
      }
      if (validade_das_posiçoes == tamanho_palavra) {
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
  else if (orientação_jogada == 'v' || orientação_jogada == 'V') {

    if (Nºjogada == 1) {
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
      // 1º: Cabe no tabuleiro
      if (linha_inicial + tamanho_palavra - 1 <= linha_index) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 2º: validade dos caracteres (ter cuidado com onde começa o i!)

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
      // 3º: Não ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[i + linha_inicial][coluna_inicial + 3][0] != '#') {
          validade_das_posiçoes++;
        }
      }
      if (validade_das_posiçoes == tamanho_palavra) {
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
      // 1º: Tem pelo menos um caracter igual;
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
      // 2º: Cabe no tabuleiro
      if (linha_inicial + tamanho_palavra - 1 <= linha_index) {
        validade++;
      } else {
        erros[1] = '1';
      }
      // 3º: validade dos caracteres (ter cuidado com onde começa o i!)

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
      // 4º: Não ocupa casas proibidas
      for (int i = 0; i < tamanho_palavra; i++) {
        if (dados[i + linha_inicial][coluna_inicial + 3][0] != '#') {
          validade_das_posiçoes++;
        }
      }
      if (validade_das_posiçoes == tamanho_palavra) {
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

// Contabilização de Pontos
// Tem apenas em contabilizaçao a palavra em si e os bonus por letra, não inclui
// os bonus de palavra
void contabilização_possivel_pontos(
    int Nºjogada, int tamanho_tabuleiro, char jogada_conteudo[],
    int Linha_jogada, char orientação_jogada, char Coluna_jogada,
    int colunas_total, int linhas_total,
    char dados[linhas_total][colunas_total][7]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;

  if (orientação_jogada == 'h' || orientação_jogada == 'H') {
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
        //               letras que contam 10 (usa : porque 10 não existe em
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

  if (orientação_jogada == 'v' || orientação_jogada == 'V') {
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
        //               letras que contam 10 (usa : porque 10 não existe em
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

// pontuação jogada
void pontuação_jogada_possivel(int Nºjogada, int tamanho_tabuleiro,
                               char jogada_conteudo[], int Linha_jogada,
                               char orientação_jogada, char Coluna_jogada,
                               int colunas_total, int linhas_total,
                               char dados[linhas_total][colunas_total][7],
                               int *pontuação_jogada, int modo_de_jogo) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra,
      pontuaçao_da_palavra, multiplicador_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;
  pontuaçao_da_palavra = 0;
  multiplicador_palavra = 1;

  if (orientação_jogada == 'h' || orientação_jogada == 'H') {
    for (int i = 0; i < tamanho_palavra; i++) {
      pontuaçao_da_palavra =
          pontuaçao_da_palavra +
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

  if (orientação_jogada == 'v' || orientação_jogada == 'V') {
    for (int i = 0; i < tamanho_palavra; i++) {
      pontuaçao_da_palavra =
          pontuaçao_da_palavra +
          (int)dados[i + linha_inicial][coluna_inicial + 3][6];
    }
    for (int i = 0; i < tamanho_palavra; i++) {
      if ((coluna_inicial + 3 == 3 || coluna_inicial + 3 == coluna_index) &&
          (i + linha_inicial == 0 || i + linha_inicial == linha_index - 2)) {
        multiplicador_palavra = multiplicador_palavra * 2;
      }
    }
  }
  *pontuação_jogada = (multiplicador_palavra * pontuaçao_da_palavra);
}
// Insere possivel jogada no tabuleiro
void Insere_possivel_jogada_tabuleiro(
    int Nºjogada, int tamanho_tabuleiro, char jogada_conteudo[],
    int Linha_jogada, char orientação_jogada, char Coluna_jogada,
    int colunas_total, int linhas_total,
    char dados[linhas_total][colunas_total][7]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;

  if (orientação_jogada == 'h' || orientação_jogada == 'H') {
    for (int i = 0; i < tamanho_palavra; i++) {
      dados[linha_inicial][i + coluna_inicial + 3][4] = jogada_conteudo[i];
    }
  }

  if (orientação_jogada == 'v' || orientação_jogada == 'V') {
    for (int i = 0; i < tamanho_palavra; i++) {
      dados[i + linha_inicial][coluna_inicial + 3][4] = jogada_conteudo[i];
    }
  }
}

// Remove posivel jogada no tabuleiro
void remove_posivel_jogada_tabuleiro(
    int Nºjogada, int tamanho_tabuleiro, char jogada_conteudo[],
    int Linha_jogada, char orientação_jogada, char Coluna_jogada,
    int colunas_total, int linhas_total,
    char dados[linhas_total][colunas_total][7]) {
  int linha_inicial, coluna_inicial, linha_index, coluna_index, tamanho_palavra;
  linha_index = linhas_total - 1;
  coluna_index = colunas_total - 1;
  tamanho_palavra = strlen(jogada_conteudo);
  coluna_inicial = ((int)tolower(Coluna_jogada)) - 97;
  linha_inicial = Linha_jogada - 1;

  if (orientação_jogada == 'h' || orientação_jogada == 'H') {
    for (int i = 0; i < tamanho_palavra; i++) {
      dados[linha_inicial][i + coluna_inicial + 3][4] = ' ';
      dados[linha_inicial][i + coluna_inicial + 3][5] = '0';
      dados[linha_inicial][i + coluna_inicial + 3][6] = '0';
    }
  }

  if (orientação_jogada == 'v' || orientação_jogada == 'V') {
    for (int i = 0; i < tamanho_palavra; i++) {
      dados[i + linha_inicial][coluna_inicial + 3][4] = ' ';
      dados[i + linha_inicial][coluna_inicial + 3][5] = '0';
      dados[i + linha_inicial][coluna_inicial + 3][6] = '0';
    }
  }
}

// fim do jogo
int fim_do_jogo(int tamanho_tabuleiro, char jogada_conteudo[], int Linha_jogada,
                char orientação_jogada, char Coluna_jogada, int colunas_total,
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

const int Tamanho_realocação = 1;
char **carregar_ficheiro(char *File_dicionario, int *NºColunas_DIC) {
  char leitura[100];
  FILE *dicionario = fopen(File_dicionario, "r");
  if (!dicionario) {
    printf("Não foi possivel abrir o ficheiro do dicionario na leitura\n");
  }
  int tamanho_dinamico = Tamanho_realocação;
  char **linhas = (char **)malloc(tamanho_dinamico * sizeof(char *));
  int i = 0;
  while (fgets(leitura, 100, dicionario)) {
      for (int x=0; x<100; x++) {
          leitura[x]=tolower(leitura[x]);
      }
    if (i == tamanho_dinamico) {
      tamanho_dinamico += Tamanho_realocação;
      char **linhasRE = realloc(linhas, tamanho_dinamico * sizeof(char *));
      if (!linhasRE) {
        printf("Não foi possivel realocar a memoria");
      }
      linhas = linhasRE;
    }
    for (int a = 0; a < 100; a++) {
      if (leitura[a] == (char)39) {
        leitura[a] = '\n';
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
  *NºColunas_DIC = i;
  return linhas;
}

// Cria string da jogada possivel
char *cria_string(char Coluna_jogada, char orientação_jogada, int Linha_jogada,
                  int pontuação_jogada, char jogada_conteudo[15]) {
  int tamanho_linha = 1;
  if (Linha_jogada >= 10) {
    tamanho_linha = 2;
  }
  int tamanho_pontuação = 1;
  if (pontuação_jogada >= 10) {
    tamanho_pontuação = 2;
  }
  int tamanho_palavra = strlen(jogada_conteudo);
  int tamanho_total = (4 + tamanho_linha + tamanho_palavra + tamanho_pontuação);
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
      jogada_possivel[i] = orientação_jogada;
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
      if (tamanho_pontuação == 1) {
        jogada_possivel[i] = '1' + (Linha_jogada - 1);
      }
      if (tamanho_pontuação == 2) {
        jogada_possivel[tamanho_total - 2] = '1' + (pontuação_jogada / 10) - 1;
        jogada_possivel[tamanho_total - 1] =
            '1' + (pontuação_jogada - 1) - ((pontuação_jogada / 10) * 10);
      }
    }
  }
  strcat(jogada_possivel, " pontos");
  return jogada_possivel;
}

// Criar array das jogadas possiveis
char **Criar_array_jogadas(int *Nºtotal_jogadas_possiveis) {
  int tamanho_dinamico = Tamanho_realocação;
  char **linhas_jogadas = (char **)malloc(tamanho_dinamico * sizeof(char *));
  int i = 0;
  *Nºtotal_jogadas_possiveis = i;
  return linhas_jogadas;
}

// Insere a jogada no array
char **Insere_jogada_possivel_array(char **Jogadas_Possiveis,
                                    int *Nºtotal_jogadas_possiveis,
                                    char *jogada_possivel, int Nºjogada,
                                    int *Nºjogada_array) {
  int i = 0;
  int tamanho_dinamico = Tamanho_realocação;
  if (i == tamanho_dinamico) {
    tamanho_dinamico += Tamanho_realocação;
    char **Jogadas_possiveisRE =
        realloc(Jogadas_Possiveis, tamanho_dinamico * sizeof(char *));
    if (!Jogadas_possiveisRE) {
      printf("Não foi possivel realocar a memoria");
    }
    Jogadas_Possiveis = Jogadas_possiveisRE;
    if (*Nºjogada_array != Nºjogada) {
      Nºjogada_array++;

      int tamanho_Nºjogada = 1;
      if (Nºjogada > 10) {
        tamanho_Nºjogada = 2;
      }

      char *NªJOGADA = (char *)malloc((tamanho_Nºjogada + 8) * sizeof(char));
      for (int a = 0; a < 2; a++) {
        if ((a == 0) || (a == 1)) {
          if (tamanho_Nºjogada == 1) {
            NªJOGADA[0] = '1' + (Nºjogada - 1);
          }
          if (tamanho_Nºjogada == 2) {
            NªJOGADA[0] = '1' + (Nºjogada / 10) - 1;
            NªJOGADA[1] = '1' + (Nºjogada - 1) - ((Nºjogada / 10) * 10);
          }
        }
      }
      strcat(NªJOGADA, "ºJogada:");
      Jogadas_Possiveis[i] = NªJOGADA;
    } else {
      Jogadas_Possiveis[i] = jogada_possivel;
    }
    i++;
  }
  *Nºtotal_jogadas_possiveis = i;
  return Jogadas_Possiveis;
}
//  Marca as casas a analisar
/* Dois modos:Horizontal e vertical
 Horizontal:
 Anda de casa em casa até encontrar uma casa com se a casa não é proibida
 nem e imendiatamente estiver uma letra em cima ou em baixo a casa é
 passada ao algoritmo
 */
void Marcar_casas(int Nºjogada, char *Coluna_jogada, char *orientação_jogada,
               int *Linha_jogada, char jogada_conteudo[15],
               int tamanho_tabuleiro, int pontuação_jogada, int NºColunas_DIC,
               char **Coluna_DIC, int colunas_total, int linhas_total,
               char dados[linhas_total][colunas_total][7], char erros[6],
               int modo_de_jogo, char **Jogadas_Possiveis,
               int *Nºtotal_jogadas_possiveis, int *Nºjogada_array) {
 
  
  int linha_inicial, coluna_inicial;
  coluna_inicial = ((int)tolower(*Coluna_jogada)) - 97;
  linha_inicial = (*Linha_jogada) - 1;
 
  // melhor jogada
  char Coluna_jogada_Max = 'c';
  char orientação_jogada_Max = 'h';
  int Linha_jogada_Max = 0;
  int pontuação_jogada_Max = 0;
    char jogada_conteudo_Max[15] ={0};

    // Horizontal

      (pontuação_jogada) = 0;
      (*Coluna_jogada) = 'A';
      (*orientação_jogada) = 'h';
      (*Linha_jogada) = 1;
      // vê coluna a coluna
      for (int x = 0; x < tamanho_tabuleiro; x++) {
        for (int i = 0; i < tamanho_tabuleiro; i++) {
          coluna_inicial = (((int)(tolower(*Coluna_jogada))) - 97) + i;
          linha_inicial = (*Linha_jogada) - 1;
          // vê se ha letra na casa
          if (Nºjogada == 1) {
            (*Coluna_jogada) = ((char)coluna_inicial) + 97 + i;
            (*Linha_jogada) = linha_inicial + 1;
          }
          int c1 = (int)dados[linha_inicial][i + coluna_inicial + 3][0];
          if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) { // define a casa
            (*Coluna_jogada) = ((char)coluna_inicial) + 97 + i;
            (*Linha_jogada) = linha_inicial + 1;
            // faz os deslocamentos das casas
            for (int b = 0; b < tamanho_tabuleiro; b++) {
              (*Coluna_jogada) = ((char)coluna_inicial) + 97 - b;
              // vai ao dicionario ver todas as palavras
              for (int a = 0; a < (NºColunas_DIC); a++) {
                jogada_conteudo = Coluna_DIC[a];
                  printf("%s\n",Coluna_DIC[a]);
                Insere_possivel_jogada_tabuleiro(
                    Nºjogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                    *orientação_jogada, *Coluna_jogada, colunas_total, linhas_total,
                    dados);
                if (validação_possivel_da_jogada(
                        Nºjogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                        *orientação_jogada, *Coluna_jogada, colunas_total,
                        linhas_total, dados, erros) == 1) {
                  contabilização_possivel_pontos(
                      Nºjogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                      *orientação_jogada, *Coluna_jogada, colunas_total,
                      linhas_total, dados);
                  pontuação_jogada_possivel(
                      Nºjogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                      *orientação_jogada, *Coluna_jogada, colunas_total,
                      linhas_total, dados, &pontuação_jogada, modo_de_jogo);
                  if (pontuação_jogada >= 0.5 * pontuação_jogada_Max) {
                    char *jogada_possivel = cria_string(
                        *Coluna_jogada, *orientação_jogada, *Linha_jogada,
                        pontuação_jogada, jogada_conteudo);
                    Jogadas_Possiveis = Insere_jogada_possivel_array(
                        Jogadas_Possiveis, Nºtotal_jogadas_possiveis,
                        jogada_possivel, Nºjogada, Nºjogada_array);
                  }
                  // Guarda a melhor jogada
                  if ((pontuação_jogada) > pontuação_jogada_Max) {
                    pontuação_jogada_Max = (pontuação_jogada);
                    Coluna_jogada_Max = (*Coluna_jogada);
                    orientação_jogada_Max = (*orientação_jogada);
                    Linha_jogada_Max = (*Linha_jogada);
                    strcpy(jogada_conteudo_Max, jogada_conteudo);
                  } // no fim de analisar a jogada remove-a
                  remove_posivel_jogada_tabuleiro(
                      Nºjogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                      *orientação_jogada, *Coluna_jogada, colunas_total,
                      linhas_total, dados);
                   jogada_conteudo=0;

                } else {
                  remove_posivel_jogada_tabuleiro(
                      Nºjogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                      *orientação_jogada, *Coluna_jogada, colunas_total,
                      linhas_total, dados);
                  (pontuação_jogada) = 0;
                  coluna_inicial = ((int)tolower(*Coluna_jogada)) - 97;
                  linha_inicial = (*Linha_jogada) - 1;
                }
              }
            (*Coluna_jogada) = ((char)coluna_inicial) + 97 - b;}
          }
         coluna_inicial = (((int)(tolower(*Coluna_jogada))) - 97) + i;
          linha_inicial = (*Linha_jogada) - 1;}
      }

  // horizontal

  (pontuação_jogada) = 0;
  (*Coluna_jogada) = 'A';
  (*orientação_jogada) = 'h';
  (*Linha_jogada) = 1;
    coluna_inicial = ((int)tolower(*Coluna_jogada)) - 97;
    linha_inicial = (*Linha_jogada) - 1;
    
    if (Nºjogada==1) {
        
    }
    else{
    for (int linha = 0; linha < tamanho_tabuleiro; linha++) {
        for (int coluna= 0; coluna < tamanho_tabuleiro; coluna++) {
            int c1 = dados[linha_inicial+linha][coluna + coluna_inicial + 3][0];
            if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122))
            {
                
            }
             
        }}}
    
    
    
    
    // vê coluna a coluna
  for (int x = 0; x < tamanho_tabuleiro; x++) {
    for (int i = 0; i < tamanho_tabuleiro; i++) {
      coluna_inicial = ((int)tolower(*Coluna_jogada)) - 97 + i;
      linha_inicial = (*Linha_jogada) - 1;
      // vê se á letra na casa
      int c1 = dados[linha_inicial][i + coluna_inicial + 3][0];
      if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) { // define a casa
        (*Coluna_jogada) = ((char)coluna_inicial) + 97 + i;
        (*Linha_jogada) = linha_inicial + 1;
        // faz os deslocamentos das casas
        for (int b = 0; b < tamanho_tabuleiro; b++) {
          (*Coluna_jogada) = ((char)coluna_inicial) + 97 - b;
          // vai ao dicionario ver todas as palavras
          for (int a = 0; a < (NºColunas_DIC); a++) {
            jogada_conteudo = Coluna_DIC[a];
            Insere_possivel_jogada_tabuleiro(
                Nºjogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                *orientação_jogada, *Coluna_jogada, colunas_total, linhas_total,
                dados);
            if (validação_possivel_da_jogada(
                    Nºjogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                    *orientação_jogada, *Coluna_jogada, colunas_total,
                    linhas_total, dados, erros) == 1) {
              contabilização_possivel_pontos(
                  Nºjogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                  *orientação_jogada, *Coluna_jogada, colunas_total,
                  linhas_total, dados);
              pontuação_jogada_possivel(
                  Nºjogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                  *orientação_jogada, *Coluna_jogada, colunas_total,
                  linhas_total, dados, &pontuação_jogada, modo_de_jogo);
              if (pontuação_jogada >= 0.5 * pontuação_jogada_Max) {
                char *jogada_possivel = cria_string(
                    *Coluna_jogada, *orientação_jogada, *Linha_jogada,
                    pontuação_jogada, jogada_conteudo);
                Jogadas_Possiveis = Insere_jogada_possivel_array(
                    Jogadas_Possiveis, Nºtotal_jogadas_possiveis,
                    jogada_possivel, Nºjogada, Nºjogada_array);
              }
              // Guarda a melhor jogada
              if ((pontuação_jogada) > pontuação_jogada_Max) {
                pontuação_jogada_Max = (pontuação_jogada);
                Coluna_jogada_Max = (*Coluna_jogada);
                orientação_jogada_Max = (*orientação_jogada);
                Linha_jogada_Max = (*Linha_jogada);
                strcpy(jogada_conteudo_Max, jogada_conteudo);
              } // no fim de analisar a jogada remove-a
              else {
                remove_posivel_jogada_tabuleiro(
                    Nºjogada, tamanho_tabuleiro, jogada_conteudo, *Linha_jogada,
                    *orientação_jogada, *Coluna_jogada, colunas_total,
                    linhas_total, dados);
              }
            }
          }
        }
      }
    }
  }

  (*Coluna_jogada) = Coluna_jogada_Max;
  (*orientação_jogada) = orientação_jogada_Max;
  (*Linha_jogada) = Linha_jogada_Max;
  strcpy(jogada_conteudo, jogada_conteudo_Max);
}

//  MAIN
int main(int argc, char *argv[]) {

  int tamanho_tabuleiro = 0, I = 0, colunas_total = 0, linhas_total = 0,
      linha_index_tabuleiro, coluna_index_tabuleiro, gameover = 0, Nºjogada = 1;
  int modo_de_jogo = 1;

  while (I == 0) {
    printf("Insira o tamanho do tabuleiro:\n");
    scanf("%d", &tamanho_tabuleiro);
    if (tamanho_tabuleiro % 2 != 0 && tamanho_tabuleiro <= 15 &&
        tamanho_tabuleiro >= 7) {
      I = 1;
    }
  }
  printf("Insira o modo de jogo:\n");
  scanf("%d", &modo_de_jogo);
  int Pontuação_total = 0;
  linhas_total = tamanho_tabuleiro + 2;
  colunas_total = tamanho_tabuleiro + 3;
  char dados[linhas_total][colunas_total][7];
  char erros[6] = {0};
  pre_formataçao(colunas_total, linhas_total, tamanho_tabuleiro, dados);
  gerar_tabuleiro(colunas_total, linhas_total, tamanho_tabuleiro, dados);
  imprimir_tabuleiro(colunas_total, linhas_total, dados);

  // modo de jogo 1
  if (modo_de_jogo == 1) {
    while (gameover == 0) {
      // jogadas variaveis
      int pontuação_jogada = 0;
      char Coluna_jogada, orientação_jogada;
      int Linha_jogada;
      char jogada_conteudo[15] = {0};
      char jogada_coordenadas[3] = {0};
      printf("Insira a sua %d jogada:\n", Nºjogada);
      scanf("%s %s", jogada_coordenadas, jogada_conteudo);
      Coluna_jogada = jogada_coordenadas[0];
      if (((int)jogada_coordenadas[2] >= 48) &&
          ((int)jogada_coordenadas[2] <= 57)) {
        Linha_jogada =
            (jogada_coordenadas[1] - 48) * 10 + jogada_coordenadas[2] - 48;
        orientação_jogada = jogada_coordenadas[3];
      } else {
        Linha_jogada = jogada_coordenadas[1] - 48;
        orientação_jogada = jogada_coordenadas[2];
      }
      if (fim_do_jogo(tamanho_tabuleiro, jogada_conteudo, Linha_jogada,
                      orientação_jogada, Coluna_jogada, colunas_total,
                      linhas_total, dados) == 1) {
        printf("A sua pontução total: %d pontos\n", Pontuação_total);
        gameover = 1;
        return 0;
      }
      if (validação_da_jogada(Nºjogada, tamanho_tabuleiro, jogada_conteudo,
                              Linha_jogada, orientação_jogada, Coluna_jogada,
                              colunas_total, linhas_total, dados, erros) == 1) {
        Insere_jogada_tabuleiro(Nºjogada, tamanho_tabuleiro, jogada_conteudo,
                                Linha_jogada, orientação_jogada, Coluna_jogada,
                                colunas_total, linhas_total, dados);
        printf("\n%dºJogada:\n", Nºjogada);
        imprimir_tabuleiro(colunas_total, linhas_total, dados);
        contabilização_de_pontos(Nºjogada, tamanho_tabuleiro, jogada_conteudo,
                                 Linha_jogada, orientação_jogada, Coluna_jogada,
                                 colunas_total, linhas_total, dados);
        printf("\n%s %s", jogada_coordenadas, jogada_conteudo);
        impressão_pontuação(Nºjogada, tamanho_tabuleiro, jogada_conteudo,
                            Linha_jogada, orientação_jogada, Coluna_jogada,
                            colunas_total, linhas_total, dados,
                            &Pontuação_total, &pontuação_jogada, modo_de_jogo);
        printf(" %d Pontos    |", pontuação_jogada);
        printf("    Pontuação total: %d\n", Pontuação_total);
        Nºjogada++;
      } else {
        printf("Invalido:\n");
        if (erros[0] == '1') {
          printf(" A primeira jogada deve ser jogada na casa central e na "
                 "horizontal\n");
          erros[0] = '0';
        }
        if (erros[1] == '1') {
          printf("A jogada não cabe no tabuleiro\n");
          erros[1] = '0';
        }
        if (erros[2] == '1') {
          printf("A palavra contêm caracteres invalidos\n");
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
          printf("A palavra não têm pelo menos um caracter igual\n");
          erros[5] = '0';
        }
      }
    }
  }

  // modo de jogo 2
  if (modo_de_jogo == 2) {
    while (gameover == 0) {
      int pontuação_jogada = 0;
      char Coluna_jogada = '0', orientação_jogada = 'h';
      int Linha_jogada = 0;
      char jogada_conteudo[15] = {0};
      int Nºjogada_array = 0;
      // leitura do dicionario
      int NºColunas_DIC = 0;
      char *File_dicionario = "/Users/pedro/Desktop/Curso/1º "
                              "ano/2ºSemestre/Programação/Laboratorio/Projecto/"
                              "dicionarios-Linux/words.txt";
      char **Coluna_DIC = carregar_ficheiro(File_dicionario, &NºColunas_DIC);

      
      int Nºtotal_jogadas_possiveis = 0;
     char **Casas_marcadas=
        Criar_jogadas_marcadas
        char **Jogadas_Possiveis =
          Criar_array_jogadas(&Nºtotal_jogadas_possiveis);
      algoritmo(Nºjogada, &Coluna_jogada, &orientação_jogada, &Linha_jogada,
                jogada_conteudo, tamanho_tabuleiro, pontuação_jogada,
                NºColunas_DIC, Coluna_DIC, colunas_total, linhas_total, dados,
                erros, modo_de_jogo, Jogadas_Possiveis,
                &Nºtotal_jogadas_possiveis, &Nºjogada_array);
      Insere_jogada_tabuleiro(Nºjogada, tamanho_tabuleiro, jogada_conteudo,
                              Linha_jogada, orientação_jogada, Coluna_jogada,
                              colunas_total, linhas_total, dados);
      printf("\n%dºJogada:\n", Nºjogada);
      imprimir_tabuleiro(colunas_total, linhas_total, dados);
      contabilização_de_pontos(Nºjogada, tamanho_tabuleiro, jogada_conteudo,
                               Linha_jogada, orientação_jogada, Coluna_jogada,
                               colunas_total, linhas_total, dados);
      printf("\n%c%d%c %s", Coluna_jogada, Linha_jogada, orientação_jogada,
             jogada_conteudo);
      impressão_pontuação(Nºjogada, tamanho_tabuleiro, jogada_conteudo,
                          Linha_jogada, orientação_jogada, Coluna_jogada,
                          colunas_total, linhas_total, dados, &Pontuação_total,
                          &pontuação_jogada, modo_de_jogo);
      printf(" %d Pontos    |", pontuação_jogada);
      printf("    Pontuação total: %d\n", Pontuação_total);
      Nºjogada++;
      gameover = 0;
      if (Nºjogada == 4) {
        gameover = 1;
      }
    }
  }
}

/* RASCUNHO:
 criar array multidimensional que tem os dados todos do jogo, o que
corresponde cada linha da matrix:
1ºdimensao:index de linha no array;
2ªdimensao:index da coluna no array; (estas duas linhas têm as coordenadas da
posição no tabuleiro)

 3ºdimensao(niveis):
 0:Conteudo da posição no tabuleiro;
 1:Bonus da posição;
 2:Pontos da letra;
 3:pontos da posiçao (1x2:bonus da posição * pontos da letra);
 4:jogada possivel a ser estudada pelo algoritmo;
 5:Pontos da letra;
 6:pontos da posiçao

 ERROS CODIGOS: ARRAY []
 1:A primeira jogada não cabe no tabuleiro;
 2:cabe no tabuleiro
 3:validade dos caracteres
 4:ocupa casas proibidas 5:a palavra é maior ou menor do que devia

 /*
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
      -n nn
      -i filename -j 1-4
      -o filename - r filename
      ajuda para o utilizador
      dimensões do tabuleiro (linha x coluna).
      nome do ficheiro de dicionário a utilizar
      nome do ficheiro com letras a usar nas jogadas
      número de letras que um jogador pode ter na sua mão para jogar
      número de jogadas máximo a realizar
      define ficheiro com o tabuleiro a usar em alternativa a jogar num
 tabuleiro vazio modo de jogo 1 a 4 define ficheiro onde escrever o
 tabuleiro final. define ficheiro para registo de todas as jogadas
 possíveis. switch (arg1[1]) { case 'h': ajuda=1; break; case 't':
 tamanho_tabuleiro= break; default: break;
     }

 // impresão do dicionario
for (int i = 0; i < NºColunas_DIC; i++) {
 printf("%s\n", Coluna_DIC[i]);

 */

