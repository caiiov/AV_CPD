
#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); // Número total de processos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);      // Rank do processo atual

    const int n = 150000000 / world_size; // Divisão do vetor entre os processos
    auto inicio_total = std::chrono::high_resolution_clock::now(); // Início do tempo total

    // Cada processo executa sua parte
    std::cout << "Processo " << rank << " preenchendo o vetor...\n";

    auto inicio_preenchimento = std::chrono::high_resolution_clock::now();
    std::vector<int> vetor(n);
    std::srand(std::time(nullptr) + rank); // Alterar a semente para cada processo
    for (int j = 0; j < n; ++j) {
        vetor[j] = std::rand() % 1000; // Valores aleatórios entre 0 e 999
    }
    auto fim_preenchimento = std::chrono::high_resolution_clock::now();
    std::cout << "Processo " << rank << " terminou de preencher o vetor.\n";

    // Ordenando o vetor
    auto inicio_ordenacao = std::chrono::high_resolution_clock::now();
    std::sort(vetor.begin(), vetor.end());
    auto fim_ordenacao = std::chrono::high_resolution_clock::now();
    std::cout << "Processo " << rank << " terminou de ordenar o vetor.\n";

    // Coletando os tempos locais de cada processo
    double tempo_preenchimento = std::chrono::duration<double>(fim_preenchimento - inicio_preenchimento).count();
    double tempo_ordenacao = std::chrono::duration<double>(fim_ordenacao - inicio_ordenacao).count();

    // Processo 0 recebe os tempos e salva no JSON
    if (rank == 0) {
        json resultados = json::array(); // Array JSON para armazenar os resultados

        // Adicionando o próprio resultado
        resultados.push_back({{"processo", rank}, {"preenchimento", tempo_preenchimento}, {"ordenacao", tempo_ordenacao}});

        // Recebendo resultados dos outros processos
        for (int i = 1; i < world_size; ++i) {
            double temp_preench, temp_ord;
            MPI_Recv(&temp_preench, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&temp_ord, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            resultados.push_back({{"processo", i}, {"preenchimento", temp_preench}, {"ordenacao", temp_ord}});
        }

        // Salvando o JSON em um arquivo
        std::ofstream file("resultado.json");
        file << resultados.dump(4); // Escrevendo com indentação
        file.close();

        std::cout << "Resultados salvos em 'resultado.json'.\n";
    } else {
        // Enviando tempos para o processo 0
        MPI_Send(&tempo_preenchimento, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&tempo_ordenacao, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
