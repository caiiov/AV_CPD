#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <pthread.h>

const int n = 150000000; // Tamanho padr�o do vetor
const int num_threads = 8; // N�mero de threads
std::vector<int> vetor(n); // Vetor global
std::vector<std::pair<int, int>> ranges(num_threads); // Faixas para cada thread

// Fun��o executada pelas threads para ordenar uma parte do vetor
void* ordenar_parte(void* arg) {
    int thread_id = *(int*)arg;
    int inicio = ranges[thread_id].first;
    int fim = ranges[thread_id].second;

    std::sort(vetor.begin() + inicio, vetor.begin() + fim); // Ordenar a parte designada
    pthread_exit(nullptr);
}

int main() {
    auto inicio_total = std::chrono::high_resolution_clock::now(); // In�cio do tempo total

    for (int i = 1; i <= 8; ++i) {
        std::cout << "Preenchendo o vetor " << i << "...\n";

        // Medir o tempo do preenchimento
        auto inicio_preenchimento = std::chrono::high_resolution_clock::now();
        std::srand(std::time(nullptr) + i); // Alterar a semente para cada preenchimento
        for (int j = 0; j < n; ++j) {
            vetor[j] = std::rand(); // Preencher com valores aleat�rios
        }
        auto fim_preenchimento = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracao_preenchimento = fim_preenchimento - inicio_preenchimento;

        std::cout << "Vetor " << i << " preenchido em " << duracao_preenchimento.count() << " segundos\n";
        std::cout << "Ordenando o vetor " << i << "...\n";

        // Dividir o vetor em partes para as threads
        int tamanho_parte = n / num_threads;
        for (int j = 0; j < num_threads; ++j) {
            ranges[j].first = j * tamanho_parte;
            ranges[j].second = (j == num_threads - 1) ? n : (j + 1) * tamanho_parte; // �ltima thread pega o restante
        }

        // Criar as threads
        pthread_t threads[num_threads];
        int thread_ids[num_threads];
        auto inicio_ordenacao = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < num_threads; ++j) {
            thread_ids[j] = j;
            pthread_create(&threads[j], nullptr, ordenar_parte, (void*)&thread_ids[j]);
        }

        // Aguardar a finaliza��o de todas as threads
        for (int j = 0; j < num_threads; ++j) {
            pthread_join(threads[j], nullptr);
        }

        auto fim_ordenacao = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracao_ordenacao = fim_ordenacao - inicio_ordenacao;

        std::cout << "Vetor " << i << " ordenado em " << duracao_ordenacao.count() << " segundos\n";
        std::cout << "Vetor " << i << " preenchido e ordenado em " << (duracao_preenchimento.count() + duracao_ordenacao.count()) << " segundos\n\n";
    }

    auto fim_total = std::chrono::high_resolution_clock::now(); // Fim do tempo total
    std::chrono::duration<double> duracao_total = fim_total - inicio_total;

    std::cout << "Tempo total de execu��o de todas as itera��es: " << duracao_total.count() << " segundos\n";

    return 0;
}

