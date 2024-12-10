#include <bits/stdc++.h>
#include <GL/glut.h> // OpenGL
#include <vector>
#include <cstdlib>
#include <iostream>
#include <utility>  
using namespace std;

random_device rd; // Biblioteca para geracao de valores randomicos
mt19937 gen(rd());
uniform_int_distribution<> dis3(1, 100);
uniform_real_distribution<> dis(0, 1.0);
uniform_real_distribution<> dis2(-1.0, 1.0);

#define tamPopulacao 100 // Quantidade de individuos
#define quantidadeCategorias 5 // Quantidade de categorias que ocorre a recomendação
#define taxaMutacao 0.01
#define limite_sup 1.0
#define limite_inf 0
#define dominio 1

struct Individuo{ // Cada indivíduo possui atributos, e o Fit, que corresponde à sua "nota" de recomendação
    float atributos[quantidadeCategorias];
    float Fit;
};

Individuo populacao[tamPopulacao]; // Inicialização da população(conjunto de indivíduos)

//float populacao[tamPopulacao][quantidadeCategorias];
//float Fit[tamPopulacao];

int melhorI = 0; // Variáveis para salvar o melhor indivíduo
float melhorV[quantidadeCategorias];


float diferenca[tamPopulacao] = {0};

float joao[quantidadeCategorias] = {0.3, 0.5, 0.1, 0.08, 0.02}; // Usuário ao qual destina-se a recomendação, com as suas preferências
float clicks [tamPopulacao][quantidadeCategorias] = {0};
float probabilidade[quantidadeCategorias] = {0};
float Ptotal = 0;
float dado = 0;
long long geracoes = 0;

float superior_temp = 1;

void inicializaPopulacao(){
    for(int i = 0; i < tamPopulacao; i++){
        Ptotal = 0;
        for(int j = 0; j < quantidadeCategorias; j++){
            populacao[i].atributos[j] = dis(gen);
            Ptotal += populacao[i].atributos[j];
        }
        for(int j = 0; j < quantidadeCategorias; j++){
            populacao[i].atributos[j] = populacao[i].atributos[j]/Ptotal;
            // Normalização dos atributos, de modo que a soma seja igual a 1
        }
    }
}

void avaliaPopulacao(){
    float min = float(INT_MAX);
    int totaisClk = 0;

    //printf("aq: %d\n", (int)(populacao[0][0]*100));
    for(int i = 0; i < tamPopulacao; i++){
        totaisClk = 0;
        diferenca[i] = 0;
        for(int j = 0; j < quantidadeCategorias; j++){
            //printf("%d ", (int)(populacao[i][j]*100));
            for(int k = 0; k < (int)(populacao[i].atributos[j]*1000)+10; k++){
                dado = dis(gen);
                if(dado <= joao[j]){
                    //printf("%f <= %f\n", dado, joao[j]);
                    clicks[i][j] = clicks[i][j] + 1;
                }
            }
        }
        //printf("\n");
    
        /*
        for(int j = 0; j < quantidadeCategorias; j++){
            totaisClk += clicks[i][j];
        }
        */

        for(int j = 0; j < quantidadeCategorias; j++){
            clicks[i][j] = clicks[i][j]/1000;
        }

        /*
        printf("Clicks %d, tem os seguintes dados: ", i);
        for(int j = 0; j < quantidadeCategorias; j++){
            printf("%f ", clicks[i][j]);
        }
        printf("\n");
        */

        for(int j = 0; j < quantidadeCategorias; j++){
            //diferenca += pow(((clicks[i][j]) - (populacao[i][j])),2);
            //diferenca += abs(clicks[i][j] - populacao[i][j]);
            diferenca[i] += pow(abs(clicks[i][j] - pow(populacao[i].atributos[j], 2)), 3);
        }

        /* CÁLCULO DO FITNESS


            obs: quanto menor o fitness, melhor a recomendação
        */

        populacao[i].Fit = diferenca[i];

        if(populacao[i].Fit < min){
            melhorI = i;
            min = populacao[i].Fit;
            for(int j = 0; j < quantidadeCategorias; j++){
                melhorV[j] = populacao[i].atributos[j];
            }
        }

    }

    for(int i = 0; i < tamPopulacao; i++){
        for(int j = 0; j < quantidadeCategorias; j++){
            clicks[i][j] = 0;
        }
    }

}


void Elitismo(){
    //Crossover
    for(int i = 0; i < tamPopulacao; i++){
        if (i == melhorI){ // Protege o melhor indivíduo
            continue;
        }

        for(int j = 0; j < quantidadeCategorias; j++){
            populacao[i].atributos[j] = (populacao[melhorI].atributos[j] + populacao[i].atributos[j])/2;
        }
    }
}

void mutacao(){
    //Mutacao
    float Ptotal1 = 0;
    float variacao = dominio * taxaMutacao;
    float mutacao;
    for(int i = 0; i < tamPopulacao; i++){
        Ptotal1 = 0;
        for(int j = 0; j < quantidadeCategorias; j++){
            mutacao = (dis2(gen))*variacao;

            //printf("mutacao: %f \n\n", mutacao);

            populacao[i].atributos[j] = populacao[i].atributos[j] + mutacao;
            if(populacao[i].atributos[j] > limite_sup){
                populacao[i].atributos[j] = 0.9999;
            }
            if(populacao[i].atributos[j] < limite_inf){
                populacao[i].atributos[j] = 0.0001;
            }
            Ptotal1 += populacao[i].atributos[j];
        }

       /*printf("Elemento %d, tem os seguintes dados: ", i);
        for(int j = 0; j < quantidadeCategorias; j++){
            printf("%f ", populacao[i][j]);
        }
        printf("\n\nP total: %f \n\n", Ptotal1);
        */

        if (Ptotal1 > 0) {
            for(int j = 0; j < quantidadeCategorias; j++){
                populacao[i].atributos[j] = populacao[i].atributos[j]/Ptotal1;
            }
        }

        /*
       printf("Elemento %d, tem os seguintes dados: ", i);
        for(int j = 0; j < quantidadeCategorias; j++){
            printf("%f ", populacao[i][j]);
        }
        printf("\n\n");
        */
    }
}

bool compararFitness(Individuo pessoa1, Individuo pessoa2) {
    return pessoa1.Fit > pessoa2.Fit; // Ordena do menos apto para o mais apto
}

#define taxa_predacao 10 // Determina a cada quantas geracoes a predacao ocorre
#define porcentagemMortos 0.10

void predacao() {

    Individuo* first(&populacao[0]);
    Individuo* last(first + tamPopulacao);

    sort(first, last, compararFitness); // Iteradores iniciais e finais, comparador
    // Ordena por fitness

    for(int i = 0; i < tamPopulacao; i++) {
        printf("Fit do %dº pior %f \n", i, populacao[i].Fit);
    }
    
    int numMortos = porcentagemMortos * tamPopulacao;

    for(int i = 0; i < numMortos; i++){ // Substitui os mais fracos hahaahah
        Ptotal = 0;
        printf("Fit do substituido %f \n", populacao[i].Fit);
        for(int j = 0; j < quantidadeCategorias; j++){
            populacao[i].atributos[j] = dis(gen);
            Ptotal += populacao[i].atributos[j];
        }
        for(int j = 0; j < quantidadeCategorias; j++){
            populacao[i].atributos[j] = populacao[i].atributos[j]/Ptotal;
        }
    }

}


void visualiza(){
    float media[quantidadeCategorias] = {0};
    for(int i = 0; i < tamPopulacao; i++){
        for(int j = 0; j < quantidadeCategorias; j++){
            media[j] += populacao[i].atributos[j];
        }
    }
    for(int j = 0; j < quantidadeCategorias; j++){
        media[j] = media[j]/tamPopulacao;
    }

    printf("\nMedia, tem os seguintes dados: ");
    for(int j = 0; j < quantidadeCategorias; j++){
        printf("%f ", media[j]);
    }
    printf("\n");

    for(int i = 0; i < tamPopulacao; i++){
        printf("Elemento %d, tem os seguintes dados: ", i);
        for(int j = 0; j < quantidadeCategorias; j++){
            printf("%f ", populacao[i].atributos[j]);
        }
        printf("\n");
    }

    printf("Melhor %d, tem os seguintes dados: ", melhorI);
    for(int j = 0; j < quantidadeCategorias; j++){
        printf("%f ", populacao[melhorI].atributos[j]);
    }
    printf("\n\n");
}

void mutacaoPersona(){
    float variacao = dominio * taxaMutacao;
    variacao = variacao*25;
    float Ptotal1 = 0;
    float mutacao;
    for(int j = 0; j < quantidadeCategorias; j++){
        mutacao = (dis2(gen))*variacao;
        //printf("mutacao: %f \n\n", mutacao);

        joao[j] = joao[j] + mutacao;
        if(joao[j]>limite_sup){
            joao[j] = 0.9999;
        }
        if(joao[j]<limite_inf){
            joao[j] = 0.0001;
        }
        Ptotal1 += joao[j];
    }

    if (Ptotal1 > 0) {
        for(int j = 0; j < quantidadeCategorias; j++){
            joao[j] = joao[j]/Ptotal1;
        }
    }

}

int main(){

    int loop_interno = 101;

    for(int i = 0; i < loop_interno; i++){
        avaliaPopulacao();
        visualiza();
        if(i % taxa_predacao == 0) {
            predacao();
        }else{
            Elitismo();
        }

        mutacao();
    }
    Individuo* first(&populacao[0]);
    Individuo* last(first + tamPopulacao);

    sort(first, last, compararFitness); // Iteradores iniciais e finais, comparador
    // Ordena por fitness

    for(int i = 0; i < tamPopulacao; i++) {
        printf("Fit do %dº pior %f \n", i, populacao[i].Fit);
    }

    return 0;
}

