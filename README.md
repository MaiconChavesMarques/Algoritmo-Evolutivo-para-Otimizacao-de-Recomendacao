# Algoritmo Evolutivo para Recomendação Balanceada

Este repositório contém a implementação de um **algoritmo evolutivo** para um sistema de recomendação balanceada, em que o objetivo é ajustar as recomendações feitas a um usuário com base nas suas preferências e interações simuladas (cliques).

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

## Como funciona a avaliação?

O algoritmo simula cliques do usuário em cada categoria com base em suas preferências. A diferença entre os cliques simulados e os valores esperados (o quadrado dos atributos do indivíduo) é usada para calcular a aptidão (`Fit`). Indivíduos com menor diferença são considerados melhores.

## Como executar

Certifique-se de ter o compilador g++ instalado e compile o código com:

```bash
g++ -o evolutivo evo9.cpp -lGL -lGLU -lglut
```
Depois de compilado, rode:
```bash
./evolutivo
```
# Aplicações

Este algoritmo pode ser usado para:
1. Sistemas de recomendação que necessitam de balanceamento entre categorias.
2. Simulações para otimizar a correspondência entre preferências de usuários e itens recomendados.
3. Estudos de algoritmos evolutivos com dinâmica de predação.

## Alunos:
<li> Henrique Yukio Sekido: </li>
<li> Karl Altenhofen Cruz: </li>
<li> Maicon Chaves Marques: </li>
