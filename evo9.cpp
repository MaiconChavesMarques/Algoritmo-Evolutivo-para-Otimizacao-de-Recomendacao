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
#define taxaMutacao 0.02
#define limite_sup 1.0
#define limite_inf 0
#define dominio 1

struct Individuo{ // Cada indivíduo possui atributos, e o Fit, que corresponde à sua "nota" de recomendação
    float atributos[quantidadeCategorias];
    float Fit;
};

Individuo populacao[tamPopulacao]; // Inicialização da população(conjunto de indivíduos)

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

    for(int i = 0; i < tamPopulacao; i++){
        totaisClk = 0;
        diferenca[i] = 0;
        for(int j = 0; j < quantidadeCategorias; j++){
            for(int k = 0; k < (int)(populacao[i].atributos[j]*1000)+50; k++){
                dado = dis(gen);
                if(dado <= joao[j]){
                    clicks[i][j] = clicks[i][j] + 1;
                }
            }
        }

        for(int j = 0; j < quantidadeCategorias; j++){
            clicks[i][j] = clicks[i][j]/(1000+(quantidadeCategorias*50));
        }

        for(int j = 0; j < quantidadeCategorias; j++){
            diferenca[i] += pow(abs(clicks[i][j] - pow(populacao[i].atributos[j], 2)), 3);
        }

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
    float Ptotal1 = 0;
    float variacao = dominio * taxaMutacao;
    float mutacao;
    for(int i = 0; i < tamPopulacao; i++){
        Ptotal1 = 0;
        for(int j = 0; j < quantidadeCategorias; j++){
            mutacao = (dis2(gen))*variacao;

            populacao[i].atributos[j] = populacao[i].atributos[j] + mutacao;
            if(populacao[i].atributos[j] > limite_sup){
                populacao[i].atributos[j] = 0.9999;
            }
            if(populacao[i].atributos[j] < limite_inf){
                populacao[i].atributos[j] = 0.0001;
            }
            Ptotal1 += populacao[i].atributos[j];
        }

        if (Ptotal1 > 0) {
            for(int j = 0; j < quantidadeCategorias; j++){
                populacao[i].atributos[j] = populacao[i].atributos[j]/Ptotal1;
            }
        }
    }
}

bool compararFitness(Individuo pessoa1, Individuo pessoa2) {
    return pessoa1.Fit > pessoa2.Fit; // Ordena do menos apto para o mais apto
}

#define taxa_predacao 10 // Determina a cada quantas geracoes a predacao ocorre
#define porcentagemMortos 0.50

void predacao(int quantidade) {

    Individuo* first(&populacao[0]);
    Individuo* last(first + tamPopulacao);

    sort(first, last, compararFitness); // Iteradores iniciais e finais, comparador
    // Ordena por fitness
    melhorI = (tamPopulacao-1);
    for(int j = 0; j < quantidadeCategorias; j++){
        melhorV[j] = populacao[melhorI].atributos[j];
    }
    
    int numMortos = (quantidade/100) * tamPopulacao;

    for(int i = 0; i < numMortos; i++){ // Substitui os mais fracos hahaahah
        Ptotal = 0;
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
    variacao = variacao*10;
    float Ptotal1 = 0;
    float mutacao;
    for(int j = 0; j < quantidadeCategorias; j++){
        mutacao = (dis2(gen))*variacao;

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



//OpenGL

#include <GL/glut.h>
#include <vector>
#include <string>
#include <utility>
#include <cstdio>
#include <cstdlib>

int numWindows = 2 * quantidadeCategorias; // Dobrar o número de janelas

// Estrutura para armazenar os dados de cada janela
struct WindowData {
    std::vector<std::pair<float, float>> redLinePoints;    // Pontos da linha vermelha
    std::vector<std::pair<float, float>> blueLinePoints;   // Pontos da linha azul
    std::vector<std::pair<float, float>> greenLinePoints;  // Pontos da linha verde (antigo)
    std::vector<std::pair<float, float>> yellowLinePoints; // Pontos da linha amarela (novo)
    std::vector<std::pair<float, float>> purpleLinePoints; // Pontos da linha roxa (novo)
    float lastRedPointX = 0.0f;
    float lastBluePointX = 0.0f;
    float lastGreenPointX = 0.0f;
    float lastYellowPointX = 0.0f;
    float lastPurplePointX = 0.0f;
};

// Lista que armazena os dados de todas as janelas
std::vector<WindowData> allWindowsData(numWindows);

// Configurações gerais
float pointMoveSpeed = 10.0f;        // Velocidade do deslocamento (em pixels)
int windowWidth = 400;              // Largura da janela
int windowHeight = 300;             // Altura da janela
float screenLimitX = 400.0f;        // Limite horizontal da tela

// Desenha as linhas de uma janela específica
void drawLines(const WindowData& windowData, bool isOldType) {
    if (isOldType) {
        // Linha vermelha
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (const auto& point : windowData.redLinePoints) {
            glVertex2f(point.first, point.second);
        }
        glEnd();

        // Linha azul
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_LINE_STRIP);
        for (const auto& point : windowData.blueLinePoints) {
            glVertex2f(point.first, point.second);
        }
        glEnd();

        // Linha verde
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (const auto& point : windowData.greenLinePoints) {
            glVertex2f(point.first, point.second);
        }
        glEnd();
    } else {
        // Linha amarela
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (const auto& point : windowData.yellowLinePoints) {
            glVertex2f(point.first, point.second);
        }
        glEnd();

        // Linha roxa
        glColor3f(1.0f, 0.0f, 1.0f);
        glBegin(GL_LINE_STRIP);
        for (const auto& point : windowData.purpleLinePoints) {
            glVertex2f(point.first, point.second);
        }
        glEnd();
    }
}

// Desloca os pontos de uma janela para a esquerda
void slideLinesLeft(WindowData& windowData) {
    for (auto& point : windowData.redLinePoints) point.first -= pointMoveSpeed;
    for (auto& point : windowData.blueLinePoints) point.first -= pointMoveSpeed;
    for (auto& point : windowData.greenLinePoints) point.first -= pointMoveSpeed;
    for (auto& point : windowData.yellowLinePoints) point.first -= pointMoveSpeed;
    for (auto& point : windowData.purpleLinePoints) point.first -= pointMoveSpeed;

    // Remove pontos fora da tela
    if (!windowData.redLinePoints.empty() && windowData.redLinePoints[0].first < 0)
        windowData.redLinePoints.erase(windowData.redLinePoints.begin());
    if (!windowData.blueLinePoints.empty() && windowData.blueLinePoints[0].first < 0)
        windowData.blueLinePoints.erase(windowData.blueLinePoints.begin());
    if (!windowData.greenLinePoints.empty() && windowData.greenLinePoints[0].first < 0)
        windowData.greenLinePoints.erase(windowData.greenLinePoints.begin());
    if (!windowData.yellowLinePoints.empty() && windowData.yellowLinePoints[0].first < 0)
        windowData.yellowLinePoints.erase(windowData.yellowLinePoints.begin());
    if (!windowData.purpleLinePoints.empty() && windowData.purpleLinePoints[0].first < 0)
        windowData.purpleLinePoints.erase(windowData.purpleLinePoints.begin());
}

// Adiciona um novo ponto a uma linha
void addPointToLine(std::vector<std::pair<float, float>>& linePoints, float newYValue, float& lastXValue) {
    float newXValue = lastXValue + pointMoveSpeed;
    linePoints.emplace_back(newXValue, newYValue);
    lastXValue = newXValue;
}

// Verifica se é necessário deslizar a tela
bool needsSlide(const WindowData& windowData) {
    return (!windowData.redLinePoints.empty() && windowData.lastRedPointX + pointMoveSpeed >= screenLimitX) ||
           (!windowData.blueLinePoints.empty() && windowData.lastBluePointX + pointMoveSpeed >= screenLimitX) ||
           (!windowData.greenLinePoints.empty() && windowData.lastGreenPointX + pointMoveSpeed >= screenLimitX) ||
           (!windowData.yellowLinePoints.empty() && windowData.lastYellowPointX + pointMoveSpeed >= screenLimitX) ||
           (!windowData.purpleLinePoints.empty() && windowData.lastPurplePointX + pointMoveSpeed >= screenLimitX);
}

// Função de exibição para uma janela específica
void displayWindow() {
    int windowIndex = glutGetWindow() - 1;
    glClear(GL_COLOR_BUFFER_BIT);

    bool isOldType = windowIndex < quantidadeCategorias;
    drawLines(allWindowsData[windowIndex], isOldType);

    glutSwapBuffers();
}

// Inicializa e cria todas as janelas
void initializeWindows() {
    for (int i = 0; i < numWindows; i++) {
        std::string windowTitle = (i < quantidadeCategorias)
                                      ? "Atributo " + std::to_string(i + 1)
                                      : "Diferenca " + std::to_string(i - quantidadeCategorias + 1);
        glutCreateWindow(windowTitle.c_str());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if (i < quantidadeCategorias) {
            gluOrtho2D(0.0, screenLimitX, 0.0, 1.0);
        } else {
            gluOrtho2D(0.0, screenLimitX, 0.0, 0.1);
        }

        glutDisplayFunc(displayWindow);
    }
}

// Manipulador de temporizador para atualizar a lógica e redesenhar as janelas
void timerUpdate(int value) {
    if (needsSlide(allWindowsData[0])) {
        for (auto& windowData : allWindowsData) {
            slideLinesLeft(windowData);
        }
    }

    for(int i = 0; i < 10; i++){
        avaliaPopulacao();
        if(i % (taxa_predacao/2) == 0) {
            predacao(10);
            avaliaPopulacao();
            Elitismo();
        }else{
            Elitismo();
        }
        mutacao();
    }

    geracoes += 10;

    float media[quantidadeCategorias] = {0};
    for (int i = 0; i < tamPopulacao; i++) {
        for (int j = 0; j < quantidadeCategorias; j++) {
            media[j] += populacao[i].atributos[j];
        }
    }
    for (int j = 0; j < quantidadeCategorias; j++) {
        media[j] = media[j] / tamPopulacao;
    }

    int j = 0;

    for (auto& windowData : allWindowsData) {
        float randomRedY = populacao[melhorI].atributos[j];
        float randomBlueY = media[j];
        float randomGreenY = joao[j];
        float randomYellowY = abs(media[j] - populacao[melhorI].atributos[j]);
        float randomPurpleY = abs(joao[j] - populacao[melhorI].atributos[j]);

        if(randomPurpleY > 0.2){
            geracoes = 10;
            predacao(95);
            avaliaPopulacao();
            Elitismo();
        }

        j++;
        if (j == quantidadeCategorias) {
            j = 0;
        }

        addPointToLine(windowData.redLinePoints, randomRedY, windowData.lastRedPointX);
        addPointToLine(windowData.blueLinePoints, randomBlueY, windowData.lastBluePointX);
        addPointToLine(windowData.greenLinePoints, randomGreenY, windowData.lastGreenPointX);
        addPointToLine(windowData.yellowLinePoints, randomYellowY, windowData.lastYellowPointX);
        addPointToLine(windowData.purpleLinePoints, randomPurpleY, windowData.lastPurplePointX);
    }

    if (geracoes % 250 == 0) {
        geracoes = 0;
        mutacaoPersona();
    }

    for (int i = 0; i < numWindows; i++) {
        glutSetWindow(i + 1);
        glutPostRedisplay();
    }

    // Configura o próximo callback do temporizador
    glutTimerFunc(200, timerUpdate, 0); // 200 ms = 5 vezes por segundo
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);

    initializeWindows();

    // Configurar o temporizador inicial
    glutTimerFunc(200, timerUpdate, 0); // 200 ms = 5 vezes por segundo

    glutMainLoop();
    return 0;
}
