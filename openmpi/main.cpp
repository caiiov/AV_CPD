#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <nlohmann/json.hpp> // Incluindo a biblioteca para JSON

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
        vetor[j] = std::rand();
    }
    auto fim_preenchimento = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_preenchimento = fim_preenchimento - inicio_preenchimento;

    std::cout << "Processo " << rank << " preenchido em " << duracao_preenchimento.count() << " segundos\n";
    std::cout << "Processo " << rank << " ordenando o vetor...\n";

    auto inicio_ordenacao = std::chrono::high_resolution_clock::now();
    std::sort(vetor.begin(), vetor.end());
    auto fim_ordenacao = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_ordenacao = fim_ordenacao - inicio_ordenacao;

    std::cout << "Processo " << rank << " ordenado em " << duracao_ordenacao.count() << " segundos\n";

    // Enviar tempos para o processo mestre
    double tempos[2] = {duracao_preenchimento.count(), duracao_ordenacao.count()};
    if (rank == 0) {
        std::vector<double> tempos_preenchimento(world_size);
        std::vector<double> tempos_ordenacao(world_size);

        MPI_Gather(&tempos[0], 1, MPI_DOUBLE, tempos_preenchimento.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Gather(&tempos[1], 1, MPI_DOUBLE, tempos_ordenacao.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Criar o objeto JSON
        json resultado_json;
        for (int i = 0; i < world_size; ++i) {
            json processo_json;
            processo_json["processo"] = i;
            processo_json["preenchimento"] = tempos_preenchimento[i];
            processo_json["ordenacao"] = tempos_ordenacao[i];
            resultado_json.push_back(processo_json);
        }

        // Exibir o JSON resultante
        std::cout << "\nResultado em formato JSON:\n" << resultado_json.dump(4) << std::endl;
    } else {
        MPI_Gather(&tempos[0], 1, MPI_DOUBLE, nullptr, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Gather(&tempos[1], 1, MPI_DOUBLE, nullptr, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    auto fim_total = std::chrono::high_resolution_clock::now(); // Fim do tempo total
    if (rank == 0) {
        std::chrono::duration<double> duracao_total = fim_total - inicio_total;
        std::cout << "\nTempo total de execução: " << duracao_total.count() << " segundos\n";
    }

    MPI_Finalize();
    return 0;
}

