# Algoritmo Evolutivo para Recomendação Balanceada

Este repositório contém a implementação de um **algoritmo evolutivo** para um sistema de recomendação balanceada, em que o objetivo é ajustar as recomendações feitas a um usuário com base nas suas preferências e interações simuladas (cliques).

![Descrição da Imagem](https://github.com/MaiconChavesMarques/Algoritmo-Evolutivo-para-Otimizacao-de-Recomendacao/blob/main/Imagens/Captura%20de%20tela%20de%202024-12-11%2016-48-30.png)

# Link do Vídeo

[https://youtu.be/x9VkD12Ihx0?si=wJPJYCvtEEg5yVfl](https://youtu.be/G6YjvQ9zC80?si=f80Wr04hFtkhcgLW)

## Descrição Geral

O algoritmo evolutivo utiliza uma abordagem baseada em seleção, cruzamento e mutação para otimizar as recomendações. Cada recomendação é representada por um **indivíduo**, que possui atributos correspondentes a categorias diferentes. Esses atributos indicam a distribuição da recomendação entre as categorias. A soma dos atributos de cada indivíduo é normalizada para garantir que sempre totalizem 1.

O algoritmo executa as seguintes etapas principais:

1. **Inicialização da população**: Gera uma população inicial de indivíduos com atributos aleatórios normalizados.
2. **Avaliação da aptidão**: Cada indivíduo é avaliado com base em sua capacidade de alinhar-se às preferências do usuário.
3. **Seleção e elitismo**: O melhor indivíduo é preservado, e os demais são ajustados para se aproximarem dele.
4. **Mutação**: Pequenas variações são aplicadas aos atributos dos indivíduos para explorar novos cenários.
5. **Predação periódica**: Substitui os indivíduos menos aptos por novos indivíduos gerados aleatoriamente.

Além disso, as preferências do usuário também podem sofrer mutações controladas, simulando mudanças dinâmicas nos interesses.

## Estrutura do Código

### Estruturas principais
- `struct Individuo`: Representa cada indivíduo, com atributos e o valor de aptidão (`Fit`).
- `populacao`: Array contendo todos os indivíduos da população.
- `joao`: Vetor de preferências do usuário-alvo da recomendação.

### Funções principais
- `inicializaPopulacao()`: Cria e normaliza a população inicial.
- `avaliaPopulacao()`: Calcula o quão bem os indivíduos se alinham às preferências do usuário.
- `Elitismo()`: Preserva o melhor indivíduo e ajusta os outros em relação a ele.
- `mutacao()`: Aplica variações aleatórias nos atributos dos indivíduos.
- `predacao()`: Remove os indivíduos menos aptos e os substitui por novos aleatórios.
- `mutacaoPersona()`: Altera as preferências do usuário, simulando variações em interesses.
- `visualiza()`: Exibe informações detalhadas da população, incluindo médias e o melhor indivíduo.

### Constantes importantes
- `tamPopulacao`: Tamanho da população.
- `quantidadeCategorias`: Número de categorias de recomendação.
- `taxaMutacao`: Taxa de mutação aplicada aos atributos dos indivíduos.
- `taxa_predacao`: Frequência com que ocorre a predação na população.
- `porcentagemMortos`: Proporção de indivíduos substituídos durante a predação.

### Gráficos
- <strong>`Atributo i`</strong>: Existem n gráficos para os n atributos do usuário. Cada gráfico representa um atributo diferente.<br><br>
A <ins>linha verde</ins> representa o valor real daquele atributo do usuário, ao qual o sistema de recomendação quer chegar. <br> 
A <ins>linha vermelha</ins> representa o valor daquele atributo do "melhor de todos" <br>
A <ins>linha azul</ins> representa o valor daquele atributo da média de todos os índividuos <br><br>

<div align="center">
  <img src="https://github.com/MaiconChavesMarques/Algoritmo-Evolutivo-para-Otimizacao-de-Recomendacao/blob/main/Imagens/Captura%20de%20tela%20de%202024-12-11%2017-10-25.png" alt="Descrição da Imagem">
</div>

- <strong>`Diferença i`</strong>: Existem n gráficos para representar as diferenças<br><br>
A <ins>linha rosa</ins> representa, em relação ao atributo i,  a diferença entre o <ins>joão</ins> e o <ins>melhor de todos</ins>.<br>
A <ins>linha amarela</ins> representa, em relação ao atributo i, a diferença entre o <ins>melhor de todos</ins> e a <ins> média</ins>.<br><br>

<div align="center">
  <img src="https://github.com/MaiconChavesMarques/Algoritmo-Evolutivo-para-Otimizacao-de-Recomendacao/blob/main/Imagens/Captura%20de%20tela%20de%202024-12-11%2017-11-40.png">
</div>


## Como funciona a avaliação?

O algoritmo simula cliques do usuário em cada categoria com base em suas preferências. A diferença entre os cliques simulados e os valores esperados (o quadrado dos atributos do indivíduo) é usada para calcular a aptidão (`Fit`). Indivíduos com menor diferença são considerados melhores.

# Instruções para Rodar o Código no Linux

Antes de iniciar, certifique-se de que os seguintes pacotes estão instalados no seu sistema:
- `g++` (compilador C++)
- `freeglut` (biblioteca OpenGL)

No Ubuntu ou distribuições baseadas no Debian, você pode instalar os pacotes necessários com o comando:
```bash
sudo apt-get update && sudo apt-get install -y g++ freeglut3 freeglut3-dev
```
## Compilando o Código

Salve o código em um arquivo com extensão .cpp, por exemplo: evo9.cpp.
Abra o terminal e navegue até o diretório onde o arquivo foi salvo.
Compile o código usando o seguinte comando:

```bash
g++ -o evolutivo evo9.cpp -lGL -lGLU -lglut
```
Este comando gera um executável chamado evolutivo.

## Executando o Programa

Depois de compilado, rode:
```bash
./evolutivo
```
Certifique-se de que os valores de variáveis como quantidadeCategorias, tamPopulacao, e estruturas como populacao e joao estão definidos no seu código ou em arquivos auxiliares, pois o código fornecido depende dessas variáveis externas.
Caso o programa retorne erros ou não funcione como esperado, revise os detalhes de inicialização dessas variáveis.

# Aplicações

Este algoritmo pode ser usado para:
1. Sistemas de recomendação que necessitam de balanceamento entre categorias.
2. Simulações para otimizar a correspondência entre preferências de usuários e itens recomendados.
3. Estudos de algoritmos evolutivos com dinâmica de predação.

## Alunos:
<li> Henrique Yukio Sekido: 14614564</li>
<li> Karl Altenhofen Cruz: 14585976</li>
<li> Maicon Chaves Marques: 14593530</li>
