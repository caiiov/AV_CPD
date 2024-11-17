#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

int main() {
    const int n = 150000000; 
    json resultado_json; 
    auto inicio_total = std::chrono::high_resolution_clock::now(); 

    for (int i = 1; i <= 8; ++i) {
        std::cout << "Preenchendo o vetor " << i << "...\n";

        auto inicio_preenchimento = std::chrono::high_resolution_clock::now();
        std::vector<int> vetor(n);

        json info_vetor;
        info_vetor["vetor"] = i;

        
        #pragma omp parallel
        {
            int num_threads = omp_get_num_threads();
            int thread_id = omp_get_thread_num();
            int chunk = n / num_threads;
            int local_init = thread_id * chunk;
            int local_end = (thread_id + 1) * chunk;

            if (thread_id == num_threads - 1) {
                local_end = n;
            }

            for (int j = local_init; j < local_end; ++j) {
                std::srand(std::time(nullptr) + i + thread_id); 
                vetor[j] = std::rand();
            }

            #pragma omp critical
            {
                info_vetor["threads"].push_back({
                    {"thread_id", thread_id},
                    {"local_init", local_init},
                    {"local_end", local_end}
                });
            }
        }

        auto fim_preenchimento = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracao_preenchimento = fim_preenchimento - inicio_preenchimento;

        info_vetor["preenchimento_tempo"] = duracao_preenchimento.count();
        std::cout << "Vetor " << i << " preenchido em " << duracao_preenchimento.count() << " segundos\n";

        std::cout << "Ordenando o vetor " << i << "...\n";

        auto inicio_ordenacao = std::chrono::high_resolution_clock::now();

        #pragma omp parallel
        {
            int num_threads = omp_get_num_threads();
            int thread_id = omp_get_thread_num();
            int chunk = n / num_threads;
            int local_init = thread_id * chunk;
            int local_end = (thread_id + 1) * chunk;

            if (thread_id == num_threads - 1) {
                local_end = n; 
            }

            std::sort(vetor.begin() + local_init, vetor.begin() + local_end);

            #pragma omp critical
            {
                info_vetor["ordenacao_threads"].push_back({
                    {"thread_id", thread_id},
                    {"local_init", local_init},
                    {"local_end", local_end}
                });
            }
        }

        auto fim_ordenacao = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracao_ordenacao = fim_ordenacao - inicio_ordenacao;

        info_vetor["ordenacao_tempo"] = duracao_ordenacao.count();
        std::cout << "Vetor ordenado em " << duracao_ordenacao.count() << " segundos\n";

        resultado_json["vetores"].push_back(info_vetor);
    }

    auto fim_total = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double> duracao_total = fim_total - inicio_total;

    resultado_json["tempo_total"] = duracao_total.count();
    std::cout << "Tempo total de execução: " << duracao_total.count() << " segundos\n";

    
    std::ofstream arquivo_json("resultadoOpenMP.json");
    arquivo_json << resultado_json.dump(4); 
    arquivo_json.close();

    std::cout << "Resultados salvos em 'resultadoOpenMP.json'.\n";

    return 0;
}
