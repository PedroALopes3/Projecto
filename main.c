//
//  main.c
//  Projecto_Palavras_Cruzadas
//
//  Created by Pedro Lopes on 25/03/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

// Pre-formatação do tabuleiro
void pre_formataçao(int colunas_total,int linhas_total,int tamanho_tabuleiro,char dados[linhas_total][colunas_total][5]){
    int linha,coluna,linha_index,coluna_index;
    linha_index=linhas_total-1;
    coluna_index=colunas_total-1;
    for (linha=0;linha<=linha_index;linha++) {
        for (coluna=0;coluna<=coluna_index;coluna++){
            if((coluna>=3)&&(coluna<=coluna_index)&&(linha>=0)&&(linha<=linha_index-2))
            {dados[linha][coluna][0]='.';dados[linha][coluna][1]='1';}
            else{dados[linha][coluna][0]=' ';dados[linha][coluna][1]='0';}
        }
    }
}

//Gerar tabuleiro
void gerar_tabuleiro(int colunas_total,int linhas_total,int tamanho_tabuleiro,char dados[linhas_total][colunas_total][5]){
    int linha,coluna,linha_index,coluna_index;
    linha_index=linhas_total-1;
    coluna_index=colunas_total-1;
    for (linha=0;linha<=linha_index;linha++) {
        for (coluna=0;coluna<=coluna_index;coluna++) {
            // extremos $
            if ((coluna==3||coluna==coluna_index)&&(linha==0||linha==linha_index-2))
            {dados[linha][coluna][0]='$';}
            // tripos 3
            else if (((coluna==3+((tamanho_tabuleiro-1)/2))&&(linha==0||linha==linha_index-2))||((coluna==3||coluna==coluna_index)&&(linha==((tamanho_tabuleiro-1)/2))))
            {dados[linha][coluna][0]='3';dados[linha][coluna][1]='3';}
            // duplos 2
            else if (((linha==coluna-3)||(linha+coluna==coluna_index))&&((linha!=0)&&(linha!=linha_index-2)&&(linha<linha_index-2)))
            {dados[linha][coluna][0]='2';dados[linha][coluna][1]='2';}
            // casas proibidas #
            else if (
                     ((((coluna==4+((tamanho_tabuleiro-1)/2))||(coluna==2+((tamanho_tabuleiro-1)/2))))&&(linha==1||linha==linha_index-3))
                     ||(
                        ( (coluna==4)||(coluna==coluna_index-1) ) && ( (linha==((tamanho_tabuleiro-1)/2)+1) || (linha==((tamanho_tabuleiro-1)/2)-1) )
                        ))
            {dados[linha][coluna][0]='#';dados[linha][coluna][1]='0';}
            //coordenadas linhas
            else if  ( (linha==linha_index) && (coluna>=3) )
            {dados[linha][coluna][0]='A'+coluna-3;}
            //coordenadas colunas
            else if (coluna==0 && linha>=9 && linha<=linha_index-2){dados[linha][coluna][0]='1';}
            else if (coluna==1 && linha>=9 && linha<=linha_index-2){dados[linha][coluna][0]='0'+linha-9;}
            else if (coluna==1 && linha>=0 && (linha<9 && linha<=linha_index-2)){dados[linha][coluna][0]='0'+linha+1;}
        }
    }
}

//imprimir o tabuleiro
void imprimir_tabuleiro(int colunas_total,int linhas_total,char dados[linhas_total][colunas_total][5]){
    int linha,coluna,linha_index,coluna_index;
    linha_index=linhas_total-1;
    coluna_index=colunas_total-1;
    for (linha=0;linha<=linha_index;linha++) {
        printf("\n");
        for (coluna=0;coluna<=coluna_index;coluna++)
        {printf("%c",dados[linha][coluna][0]);
            if((coluna>=3)&&(coluna<=coluna_index)&&(linha>=0)&&(linha<=linha_index)){printf(" ");}
        }
    }
    printf("\n");
}


//Validação da jogada
int validação_da_jogada(int Nºjogada,int tamanho_tabuleiro,char jogada_conteudo [],int Linha_jogada,char orientação_jogada,
                        char Coluna_jogada,int colunas_total,int linhas_total,char dados[linhas_total][colunas_total][5]){
    int linha_inicial,coluna_inicial,linha_index,coluna_index,tamanho_palavra,validade=0,validade_dos_caracteres=0,validade_das_posiçoes=0,igualdade_caracteres=0;
    linha_index=linhas_total-1;
    coluna_index=colunas_total-1;
    tamanho_palavra=strlen(jogada_conteudo);
    coluna_inicial=((int)tolower(Coluna_jogada))-97;
    linha_inicial=Linha_jogada-1;
    //    Se é horizontal:
    
    if (orientação_jogada=='h'||orientação_jogada=='H') {
        
        //        TESTE DOS DIFERENTES CRITERIOS:
        
        if (Nºjogada==1){
            
            if (coluna_inicial!=3+((tamanho_tabuleiro-1)/2)&&linha_inicial!=((tamanho_tabuleiro-1)/2)){return 0;}
            //1º: Cabe no tabuleiro
            if (Coluna_jogada+tamanho_palavra<='A'+coluna_index-3 || Coluna_jogada+tamanho_palavra<='a'+coluna_index-3)
            {validade++;}
            //2º: validade dos caracteres (ter cuidado com onde começa o i!)
            
            for (int i=0; i<tamanho_palavra; i++) {
                int c1=(int)jogada_conteudo [i];
                if ((c1>=65&&c1<=90)||(c1>=97&&c1<=122))
                {validade_dos_caracteres++;}}
            if (validade_dos_caracteres==tamanho_palavra)
            {validade++;}
            //3º: Não ocupa casas proibidas
            for (int i=0;i<tamanho_palavra;i++)
            {if(dados[linha_inicial][i+coluna_inicial+3][0]!='#'){validade_das_posiçoes++;}}
            if (validade_das_posiçoes==tamanho_palavra)
            {validade++;}
            //        4ª comprimento
            if (tamanho_palavra>=2&&tamanho_palavra<=tamanho_tabuleiro)
            {validade++;}
            if (validade==4)
            {return 1;}
            else{return 0;}
        }
        
        else{
            //0º: Tem pelo menos um caracter igual;
            for (int i=0;i<tamanho_palavra;i++)
            {if(dados[linha_inicial][i+coluna_inicial+3][0]==jogada_conteudo[i]){igualdade_caracteres++;}}
            if (igualdade_caracteres>0)
            {validade++;}
            //1º: Cabe no tabuleiro
            if (Coluna_jogada+tamanho_palavra<='A'+coluna_index-3 || Coluna_jogada+tamanho_palavra<='a'+coluna_index-3)
            {validade++;}
            //2º: validade dos caracteres (ter cuidado com onde começa o i!)
            
            for (int i=0; i<tamanho_palavra; i++) {
                int c1=(int)jogada_conteudo [i];
                if ((c1>=65&&c1<=90)||(c1>=97&&c1<=122))
                {validade_dos_caracteres++;}}
            if (validade_dos_caracteres==tamanho_palavra)
            {validade++;}
            //3º: Não ocupa casas proibidas
            for (int i=0;i<tamanho_palavra;i++)
            {if(dados[linha_inicial][i+coluna_inicial+3][0]!='#'){validade_das_posiçoes++;}}
            if (validade_das_posiçoes==tamanho_palavra)
            {validade++;}
            //        4ª comprimento
            if ((tamanho_palavra>=2)&&(tamanho_palavra<=tamanho_tabuleiro))
            {validade++;}
            
            if (validade==5)
            {return 1;}
            else{return 0;}
        }
    }
    // Se é vertical
    else if (orientação_jogada=='v'||orientação_jogada=='V') {
        
        if (Nºjogada==1){
            if (coluna_inicial!=3+((tamanho_tabuleiro-1)/2)&&linha_inicial!=((tamanho_tabuleiro-1)/2)){return 0;}
            
            //1º: Cabe no tabuleiro
            if (linha_inicial+tamanho_palavra<=linha_index-2)
            {validade++;}
            //2º: validade dos caracteres (ter cuidado com onde começa o i!)
            
            for (int i=0; i<tamanho_palavra; i++) {
                int c1=(int)jogada_conteudo [i];
                if ((c1>=65&&c1<=90)||(c1>=97&&c1<=122))
                {validade_dos_caracteres++;}}
            if (validade_dos_caracteres==tamanho_palavra)
            {validade++;}
            //3º: Não ocupa casas proibidas
            for (int i=0;i<tamanho_palavra;i++)
            {if(dados[i+linha_inicial][coluna_inicial+3][0]!='#'){validade_das_posiçoes++;}}
            if (validade_das_posiçoes==tamanho_palavra)
            {validade++;}
            //        4ª comprimento
            if (tamanho_palavra>=2&&tamanho_palavra<=tamanho_tabuleiro)
            {validade++;}
            if (validade==4)
            {return 1;}
            else{return 0;}
        }
        
        else  {
            //0º: Tem pelo menos um caracter igual;
            for (int i=0;i<tamanho_palavra;i++)
            {if(dados[i+linha_inicial][coluna_inicial+3][0]==jogada_conteudo[i]){igualdade_caracteres++;}}
            if (igualdade_caracteres>0)
            {validade++;}
            //1º: Cabe no tabuleiro
            if (linha_inicial+tamanho_palavra<=linha_index-2)
            {validade++;}
            //2º: validade dos caracteres (ter cuidado com onde começa o i!)
            
            for (int i=0; i<tamanho_palavra; i++) {
                int c1=(int)jogada_conteudo [i];
                if ((c1>=65&&c1<=90)||(c1>=97&&c1<=122))
                {validade_dos_caracteres++;}}
            if (validade_dos_caracteres==tamanho_palavra)
            {validade++;}
            //3º: Não ocupa casas proibidas
            for (int i=0;i<tamanho_palavra;i++)
            {if(dados[i+linha_inicial][coluna_inicial+3][0]!='#'){validade_das_posiçoes++;}}
            if (validade_das_posiçoes==tamanho_palavra)
            {validade++;}
            //        4ª comprimento
            if (tamanho_palavra>=2 && tamanho_palavra<=tamanho_tabuleiro)
            {validade++;}
            if (validade==5)
            {return 1;}
            else{return 0;}
        }
    } else{return 0;}
}


//Insere jogada no tabuleiro
void Insere_jogada_tabuleiro(int Nºjogada,int tamanho_tabuleiro,char jogada_conteudo [],int Linha_jogada,char orientação_jogada,
                             char Coluna_jogada,int colunas_total,int linhas_total,char dados[linhas_total][colunas_total][5]){
    int linha_inicial,coluna_inicial,linha_index,coluna_index,tamanho_palavra;
    linha_index=linhas_total-1;
    coluna_index=colunas_total-1;
    tamanho_palavra=strlen(jogada_conteudo);
    coluna_inicial=((int)tolower(Coluna_jogada))-97;
    linha_inicial=Linha_jogada-1;
    
    if (orientação_jogada=='h'||orientação_jogada=='H') {
        for (int i=0;i<tamanho_palavra;i++)
        {dados[linha_inicial][i+coluna_inicial+3][0]=jogada_conteudo[i];}}
    
    if (orientação_jogada=='v'||orientação_jogada=='V') {
        for (int i=0;i<tamanho_palavra;i++)
        {dados[i+linha_inicial][coluna_inicial+3][0]=jogada_conteudo[i];}}
}

// Contabilização de Pontos
void contabilização_de_pontos(int Nºjogada,int tamanho_tabuleiro,char jogada_conteudo [],int Linha_jogada,char orientação_jogada,
                              char Coluna_jogada,int colunas_total,int linhas_total,char dados[linhas_total][colunas_total][5]){
    int linha_inicial,coluna_inicial,linha_index,coluna_index,tamanho_palavra;
    linha_index=linhas_total-1;
    coluna_index=colunas_total-1;
    tamanho_palavra=strlen(jogada_conteudo);
    coluna_inicial=((int)tolower(Coluna_jogada))-97;
    linha_inicial=Linha_jogada-1;
    
    
    if (orientação_jogada=='h'||orientação_jogada=='H') {
        for (int i=0;i<tamanho_palavra;i++)
        {switch (tolower(jogada_conteudo[i])) {
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
                dados[linha_inicial][i+coluna_inicial+3][2]='1';
                break;
                //               letras que contam 2
            case 'd':
            case 'g':
                dados[linha_inicial][i+coluna_inicial+3][2]='2';
                break;
                //               letras que contam 3
            case 'b':
            case 'c':
            case 'm':
            case 'p':
                dados[linha_inicial][i+coluna_inicial+3][2]='3';
                break;
                //               letras que contam 4
            case 'f':
            case 'h':
            case 'v':
            case 'w':
            case 'y':
                dados[linha_inicial][i+coluna_inicial+3][2]='4';
                break;
                //               letras que contam 5
            case 'k':
                dados[linha_inicial][i+coluna_inicial+3][2]='5';
                break;
                //               letras que contam 8
            case 'j':
            case 'x':
                dados[linha_inicial][i+coluna_inicial+3][2]='8';
                break;
                //               letras que contam 10 (usa : porque 10 não existe em codigo ascci,e : é o caracter aseguir a 9)
            case 'q':
            case 'z':
                dados[linha_inicial][i+coluna_inicial+3][2]=':';
                break;
            default:
                dados[linha_inicial][i+coluna_inicial+3][2]='0';
            break;}}
        
        for (int i=0;i<tamanho_palavra;i++){
            dados[linha_inicial][i+coluna_inicial+3][3]=((int)(dados[linha_inicial][i+coluna_inicial+3][1])-48)*((int)(dados[linha_inicial][i+coluna_inicial+3][2])-48);
            if (dados[linha_inicial][i+coluna_inicial+3][1]=='2'||dados[linha_inicial][i+coluna_inicial+3][1]=='3')
            {dados[linha_inicial][i+coluna_inicial+3][1]='1';}}
    }
    
    
    if (orientação_jogada=='v'||orientação_jogada=='V') {
        for (int i=0;i<tamanho_palavra;i++)
        {switch (tolower(jogada_conteudo[i])) {
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
                dados[i+linha_inicial][coluna_inicial+3][2]='1';
                break;
                //               letras que contam 2
            case 'd':
            case 'g':
                dados[i+linha_inicial][coluna_inicial+3][2]='2';
                break;
                //               letras que contam 3
            case 'b':
            case 'c':
            case 'm':
            case 'p':
                dados[i+linha_inicial][coluna_inicial+3][2]='3';
                break;
                //               letras que contam 4
            case 'f':
            case 'h':
            case 'v':
            case 'w':
            case 'y':
                dados[i+linha_inicial][coluna_inicial+3][2]='4';
                break;
                //               letras que contam 5
            case 'k':
                dados[i+linha_inicial][coluna_inicial+3][2]='5';
                break;
                //               letras que contam 8
            case 'j':
            case 'x':
                dados[i+linha_inicial][coluna_inicial+3][2]='8';
                break;
                //               letras que contam 10 (usa : porque 10 não existe em codigo ascci,e : é o caracter aseguir a 9)
            case 'q':
            case 'z':
                dados[i+linha_inicial][coluna_inicial+3][2]=':';
                break;
            default:
                dados[i+linha_inicial][coluna_inicial+3][2]='0';
            break;}}
        
        for (int i=0;i<tamanho_palavra;i++){
            dados[i+linha_inicial][coluna_inicial+3][3]=((int)(dados[i+linha_inicial][coluna_inicial+3][1])-48)*((int)(dados[i+linha_inicial][coluna_inicial+3][2])-48);
            if (dados[i+linha_inicial][coluna_inicial+3][1]=='2'||dados[i+linha_inicial][coluna_inicial+3][1]=='3')
            {dados[i+linha_inicial][coluna_inicial+3][1]='1';}}}
}
// imprimir pontuação
void impressão_pontuação(int Nºjogada,int tamanho_tabuleiro,char jogada_conteudo [],int Linha_jogada,char orientação_jogada,
                         char Coluna_jogada,int colunas_total,int linhas_total,char dados[linhas_total][colunas_total][5]){
    int linha_inicial,coluna_inicial,linha_index,coluna_index,tamanho_palavra,pontuaçao_da_palavra,multiplicador_palavra;
    linha_index=linhas_total-1;
    coluna_index=colunas_total-1;
    tamanho_palavra=strlen(jogada_conteudo);
    coluna_inicial=((int)tolower(Coluna_jogada))-97;
    linha_inicial=Linha_jogada-1;
    pontuaçao_da_palavra=0;
    multiplicador_palavra=1;
    
    if (orientação_jogada=='h'||orientação_jogada=='H') {
        for (int i=0;i<tamanho_palavra;i++)
        {pontuaçao_da_palavra=pontuaçao_da_palavra+(int)dados[linha_inicial][i+coluna_inicial+3][3];}
        for (int i=0;i<tamanho_palavra;i++)
        {if((i+coluna_inicial+3==3||i+coluna_inicial+3==coluna_index)&&(linha_inicial==0||linha_inicial==linha_index-2)){multiplicador_palavra=multiplicador_palavra*2;}}
    }
    
    
    if (orientação_jogada=='v'||orientação_jogada=='V') {
        for (int i=0;i<tamanho_palavra;i++)
        {pontuaçao_da_palavra=pontuaçao_da_palavra+(int)dados[i+linha_inicial][coluna_inicial+3][3];}
        for (int i=0;i<tamanho_palavra;i++)
        {if((coluna_inicial+3==3||coluna_inicial+3==coluna_index)&&(i+linha_inicial==0||i+linha_inicial==linha_index-2)){multiplicador_palavra=multiplicador_palavra*2;}}
    }
    printf(" %d pontos\n",multiplicador_palavra*pontuaçao_da_palavra);
}
//  MAIN
int main(){
    
    int tamanho_tabuleiro=7,I=0,colunas_total,linhas_total,linha_index_tabuleiro,coluna_index_tabuleiro,gameover=0,Nºjogada=1;
    while (I==0) {
        printf("Insira o tamanho do tabuleiro:\n");
        scanf("%d",&tamanho_tabuleiro);
        if (tamanho_tabuleiro%2!=0&&tamanho_tabuleiro<=15&&tamanho_tabuleiro>=7) {I=1;}
    }
    linhas_total=tamanho_tabuleiro+2;
    colunas_total=tamanho_tabuleiro+3;
    char dados[linhas_total][colunas_total][5];
    pre_formataçao(colunas_total,linhas_total,tamanho_tabuleiro,dados);
    gerar_tabuleiro(colunas_total,linhas_total,tamanho_tabuleiro,dados);
    imprimir_tabuleiro(colunas_total,linhas_total,dados);
    
    //    jogadas variaveis
    char Coluna_jogada,orientação_jogada;
    int Linha_jogada;
    while (gameover==0) {
        char jogada_conteudo [15]={0};
        char jogada_coordenadas [3]={0};
        printf("Insira a sua %d jogada:\n",Nºjogada);
        scanf("%3s %15s",jogada_coordenadas,jogada_conteudo);
        Coluna_jogada=jogada_coordenadas[0];
        Linha_jogada=jogada_coordenadas[1]-48;
        orientação_jogada=jogada_coordenadas[2];
        if(validação_da_jogada(Nºjogada,tamanho_tabuleiro,jogada_conteudo,Linha_jogada,orientação_jogada,Coluna_jogada,colunas_total,linhas_total,dados)==1){
            Insere_jogada_tabuleiro(Nºjogada,tamanho_tabuleiro,jogada_conteudo,Linha_jogada,orientação_jogada,Coluna_jogada,colunas_total,linhas_total,dados);
            imprimir_tabuleiro(colunas_total,linhas_total,dados);
            contabilização_de_pontos(Nºjogada,tamanho_tabuleiro,jogada_conteudo,Linha_jogada,orientação_jogada,Coluna_jogada,colunas_total,linhas_total,dados);
            printf("\n%s %s",jogada_coordenadas,jogada_conteudo);
            impressão_pontuação(Nºjogada,tamanho_tabuleiro,jogada_conteudo,Linha_jogada,orientação_jogada,Coluna_jogada,colunas_total,linhas_total,dados);
            Nºjogada++;}
        else{printf("Invalido\n");}
    }
}
/*
 criar array multidimensional que tem os dados todos do jogo, o que corresponde cada linha da matrix:
 1ºdimensao:index de linha no array;
 2ªdimensao:index da coluna no array; (estas duas linhas têm as coordenadas da posição no tabuleiro)
 3ºdimensao(niveis):
 0:Conteudo da posição no tabuleiro;
 1:Bonus da posição;
 2:Pontos da letra;
 3:pontos da posiçao
 */

