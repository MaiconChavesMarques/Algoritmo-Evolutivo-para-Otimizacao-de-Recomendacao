#include <bits/stdc++.h>
#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <utility>  
using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dis3(1, 100);
uniform_real_distribution<> dis(0, 1.0);
uniform_real_distribution<> dis2(-1.0, 1.0);

#define tamPopulacao 100
#define quantidadeCategorias 5
#define taxaMutacao 0.01
#define limite_sup 1.0
#define limite_inf 0
#define dominio 1
float populacao[tamPopulacao][quantidadeCategorias];
float Fit[tamPopulacao];
int melhorI = 0;
float melhorV[quantidadeCategorias];
float diferenca[tamPopulacao] = {0};

float joao[quantidadeCategorias] = {0.3, 0.5, 0.1, 0.08, 0.02};
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
            populacao[i][j] = dis(gen);
            Ptotal += populacao[i][j];
        }
        for(int j = 0; j < quantidadeCategorias; j++){
            populacao[i][j] = populacao[i][j]/Ptotal;
        }
        /*
        populacao[i][0] = 0.3;
        populacao[i][1] = 0.6;
        populacao[i][2] = 0.1;
        */
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
            for(int k = 0; k < (int)(populacao[i][j]*1000)+10; k++){
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
            diferenca[i] += pow(abs(clicks[i][j] - pow(populacao[i][j], 2)), 3);
        }

        Fit[i] = diferenca[i];

        if(Fit[i]<min){
            melhorI = i;
            min = Fit[i];
            for(int j = 0; j < quantidadeCategorias; j++){
                melhorV[j] = populacao[i][j];
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
            populacao[i][j] = (populacao[melhorI][j] + populacao[i][j])/2;
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

            populacao[i][j] = populacao[i][j] + mutacao;
            if(populacao[i][j]>limite_sup){
                populacao[i][j] = 0.9999;
            }
            if(populacao[i][j]<limite_inf){
                populacao[i][j] = 0.0001;
            }
            Ptotal1 += populacao[i][j];
        }

       /*printf("Elemento %d, tem os seguintes dados: ", i);
        for(int j = 0; j < quantidadeCategorias; j++){
            printf("%f ", populacao[i][j]);
        }
        printf("\n\nP total: %f \n\n", Ptotal1);
        */

        if (Ptotal1 > 0) {
            for(int j = 0; j < quantidadeCategorias; j++){
                populacao[i][j] = populacao[i][j]/Ptotal1;
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


void visualiza(){
    float media[quantidadeCategorias] = {0};
    for(int i = 0; i < tamPopulacao; i++){
        for(int j = 0; j < quantidadeCategorias; j++){
            media[j] += populacao[i][j];
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
            printf("%f ", populacao[i][j]);
        }
        printf("\n");
    }

    printf("Melhor %d, tem os seguintes dados: ", melhorI);
    for(int j = 0; j < quantidadeCategorias; j++){
        printf("%f ", populacao[melhorI][j]);
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

/*
int main(){

    int loop_interno = 10;

    for(int i = 0; i < loop_interno; i++){
        avaliaPopulacao();
        visualiza();
        Elitismo();
        mutacao();
    }

    return 0;
}
*/

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
                                      ? "Janela Antiga " + std::to_string(i + 1)
                                      : "Janela Nova " + std::to_string(i - quantidadeCategorias + 1);
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

    int j = 0;

    for (int i = 0; i < 10; i++) {
        avaliaPopulacao();
        Elitismo();
        mutacao();
    }

    geracoes += 10;

    float media[quantidadeCategorias] = {0};
    for (int i = 0; i < tamPopulacao; i++) {
        for (int j = 0; j < quantidadeCategorias; j++) {
            media[j] += populacao[i][j];
        }
    }
    for (int j = 0; j < quantidadeCategorias; j++) {
        media[j] = media[j] / tamPopulacao;
    }

    for (auto& windowData : allWindowsData) {
        float randomRedY = populacao[melhorI][j];
        float randomBlueY = media[j];
        float randomGreenY = joao[j];
        float randomYellowY = abs(media[j] - populacao[melhorI][j]);
        float randomPurpleY = abs(joao[j] - populacao[melhorI][j]);

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

    if (geracoes % 500 == 0) {
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
