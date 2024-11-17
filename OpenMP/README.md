# README

## Descrição dos Arquivos
### openmp.cpp
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
```docker build -t openmp-project```
2. Executar o container Docker:
```docker run openmp-project .```
### Sem Docker
1. Certifique-se de ter um compilador C++ com suporte a OpenMP instalado.
2. Certifique que a biblioteca de Json está no mesmo diretório.
3. Compile o programa:
``` g++ -o exec openMP.cpp -fopenmp    ```
4. Execute o programa:
```./exec```

## Observações
- Os resultados são salvos no arquivo resultadoOpenMP.json após a execução.
