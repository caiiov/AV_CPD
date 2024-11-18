# README

## Descrição dos Arquivos
### Nossa lógica para a implementaçao
Um programa em C++ que:
1. Gera 8 vetores com números aleatórios, utilizando OpenMP para paralelização. (tamanho 150000000, assim gerando 12*10⁸ numeros)
2. Ordena esses vetores de forma paralela.
3. Mede o tempo de execução de cada operação.
4. Salva os resultados, incluindo detalhes de desempenho e atividade das threads, em um arquivo JSON (resultadoOpenMP.json).
---
## Pré-requisitos
- Docker (para executar via container).
- Compilador C++ com suporte a OpenMP (caso queira rodar localmente).
- Bibliotecas:
    - OpenMP
    - nlohmann/json para manipulação de JSON. (Está nos arquivos)
---
### Como Compilar e Executar
### Utilizando Docker
1. Construir a imagem Docker:
```docker build -t Nome do Arquivo-project```
2. Executar o container Docker:
```docker run  Nome do Arquivo-project .```
### Sem Docker (OpenMP)
1. Certifique-se de ter um compilador C++ com suporte a OpenMP instalado.
2. Certifique que a biblioteca de Json está no mesmo diretório.
3. Compile o programa:
``` g++ -o exec openMP.cpp -fopenmp    ```
4. Execute o programa:
```./exec```
---
### Sem Docker (PThreads)
1. Certifique-se de ter um compilador C++ com suporte a OpenMP instalado.
2. Certifique que a biblioteca de Json está no mesmo diretório.
3. Compile o programa:
``` g++ -o exec Xthreads.cpp -lpthread    ```
4. Execute o programa:
```./exec```
## Observações
- Os resultados são salvos em um arquivo json após a execução.

> No começo Deus fez o homem, e o homem acha que fez Deus, e no fim deus foi morto pelo homem, e o homem agora quer ser deus. 