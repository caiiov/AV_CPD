#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <fstream> 
#include "json.hpp"


using json = nlohmann::json;

const int n = 150000000; 
const int num_threads = 8; 
std::vector<int> vetor(n); 
std::vector<std::pair<int, int>> ranges(num_threads); 


void* ordenar_parte(void* arg) {
    int thread_id = *(int*)arg;
    int inicio = ranges[thread_id].first;
    int fim = ranges[thread_id].second;

    std::sort(vetor.begin() + inicio, vetor.begin() + fim); 
    pthread_exit(nullptr);
}

int main() {
    auto inicio_total = std::chrono::high_resolution_clock::now(); 

    json resultado_json;
    for (int i = 1; i <= 8; ++i) {
        std::cout << "Preenchendo o vetor " << i << "...\n";

        
        auto inicio_preenchimento = std::chrono::high_resolution_clock::now();
        std::srand(std::time(nullptr) + i); 
        for (int j = 0; j < n; ++j) {
            vetor[j] = std::rand();
        }
        auto fim_preenchimento = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracao_preenchimento = fim_preenchimento - inicio_preenchimento;

        std::cout << "Vetor " << i << " preenchido em " << duracao_preenchimento.count() << " segundos\n";
        std::cout << "Ordenando o vetor " << i << "...\n";

        
        int tamanho_parte = n / num_threads;
        for (int j = 0; j < num_threads; ++j) {
            ranges[j].first = j * tamanho_parte;
            ranges[j].second = (j == num_threads - 1) ? n : (j + 1) * tamanho_parte;
        }

    
        pthread_t threads[num_threads];
        int thread_ids[num_threads];
        auto inicio_ordenacao = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < num_threads; ++j) {
            thread_ids[j] = j;
            pthread_create(&threads[j], nullptr, ordenar_parte, (void*)&thread_ids[j]);
        }

       
        for (int j = 0; j < num_threads; ++j) {
            pthread_join(threads[j], nullptr);
        }

        auto fim_ordenacao = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracao_ordenacao = fim_ordenacao - inicio_ordenacao;

        std::cout << "Vetor " << i << " ordenado em " << duracao_ordenacao.count() << " segundos\n";
        std::cout << "Vetor " << i << " preenchido e ordenado em " << (duracao_preenchimento.count() + duracao_ordenacao.count()) << " segundos\n\n";

      
        resultado_json["iteracoes"].push_back({
            {"id", i},
            {"tempo_preenchimento", duracao_preenchimento.count()},
            {"tempo_ordenacao", duracao_ordenacao.count()},
            {"tempo_total", duracao_preenchimento.count() + duracao_ordenacao.count()}
        });
    }

    auto fim_total = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double> duracao_total = fim_total - inicio_total;

    std::cout << "Tempo total de execução de todas as iterações: " << duracao_total.count() << " segundos\n";

    
    resultado_json["tempo_total_execucao"] = duracao_total.count();

   
    std::ofstream arquivo("resultado.json");
    arquivo << resultado_json.dump(4); 
    arquivo.close();

    std::cout << "Resultado armazenado no arquivo 'resultado.json'.\n";

    return 0;
}
