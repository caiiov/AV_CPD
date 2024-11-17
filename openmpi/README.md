# Ordenação Paralela com MPI

Este projeto demonstra a utilização da biblioteca **MPI (Message Passing Interface)** para realizar a ordenação de vetores de forma paralela em múltiplos processos. O código foi implementado em **C++** e utiliza a biblioteca **nlohmann/json** para gerar e exibir um resultado final em formato JSON. O código foi testado no sistema operacional **Linux Mint**.

## O que o código faz

O código cria um vetor de números aleatórios em cada processo paralelo, preenche esse vetor, ordena-o e mede o tempo de execução de cada uma dessas etapas. Ao final, o código coleta os tempos de execução dos processos e gera um objeto JSON contendo as informações sobre os tempos de **preenchimento** e **ordenação** para cada processo.

### Passos realizados pelo código:
1. **Inicialização do MPI**: O código começa inicializando o MPI e determina o número total de processos e o "rank" de cada processo.
2. **Preenchimento do vetor**: Cada processo preenche seu próprio vetor com números aleatórios. O número de elementos é distribuído entre os processos de forma igual.
3. **Ordenação**: Cada processo ordena seu vetor localmente.
4. **Coleta de resultados**: Os tempos de preenchimento e ordenação de cada processo são coletados no processo mestre (rank 0) através da função `MPI_Gather`.
5. **Criação de JSON**: O processo mestre cria um objeto JSON contendo os tempos de execução para cada processo.
6. **Exibição do resultado**: O JSON gerado é impresso no terminal, mostrando os tempos de execução de cada processo.

## Como executar o código

### Pré-requisitos
Certifique-se de que você possui as seguintes bibliotecas instaladas:

1. **MPI** (Message Passing Interface) - necessário para a comunicação entre os processos.
2. **nlohmann/json** - biblioteca para manipulação de JSON em C++.

Para instalar as dependências no **Linux Mint**:

```bash
sudo apt-get install libmpi-dev
sudo apt-get install nlohmann-json3-dev


## Compilação
Para compilar o código, use o comando mpicxx:

mpicxx -o main main.cpp

##Execução

Para executar o código, utilize o comando mpirun. O número de processos pode ser ajustado conforme necessário. No exemplo abaixo, o código é executado com 3 processos:

mpirun -np 3 ./main

