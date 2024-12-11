#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <mutex>
#include <condition_variable>
#include <map>
#include <climits>
#include <iomanip>  // Para manipulação de formato da tabela

// Controle de sincronização
std::mutex mtx;
std::condition_variable cv;
bool ready = false;

std::map<int, std::vector<int>> tempos_voltas; // Armazena o tempo de cada volta para cada corredor

void corredor(int id, int num_voltas) {
    // Gerador de números aleatórios para simular o tempo de corrida
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 3); // Tempo de cada volta em segundos

    for (int volta = 1; volta <= num_voltas; ++volta) {
        // Aguarda o sinal para começar a volta
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [] { return ready; });
        }

        int tempo_volta = dist(gen);
        tempos_voltas[id].push_back(tempo_volta);

        // Simula o tempo de corrida para a volta
        std::this_thread::sleep_for(std::chrono::seconds(tempo_volta));
    }
}

void exibir_tabela(int num_corredores, int num_voltas) {
    // Exibe o status de cada volta em forma de tabela
    std::cout << "\n";
    
    // Cabeçalho da tabela
    std::cout << "| Volta  |";
    for (int i = 1; i <= num_corredores; ++i) {
        std::cout << " Corredor " << i << "  |";
    }
    std::cout << "\n";
    
    // Linha de separação
    std::cout << "|--------|";
    for (int i = 1; i <= num_corredores; ++i) {
        std::cout << "-------------|";
    }
    std::cout << "\n";

    for (int volta = 1; volta <= num_voltas; ++volta) {
        std::cout << "|Volta" << std::setw(2) << volta << " |";  // Alinha a volta com 2 espaços
        
        for (int i = 1; i <= num_corredores; ++i) {
            if (tempos_voltas[i].size() >= volta) {
                std::cout << " " << std::setw(6) << tempos_voltas[i][volta - 1] << " s   |";  // Exibe o tempo da volta
            } else {
                std::cout << "   Não     |";  // Indica que o corredor ainda não completou a volta
            }
        }
        std::cout << "\n";
    }
}

int main() {
    int num_corredores, num_voltas, aposta;

    // Interação inicial com o usuário
    std::cout << "Bem-vindo ao jogo de corrida!\n";
    std::cout << "Digite o número de corredores: ";
    std::cin >> num_corredores;
    std::cout << "Digite o número de voltas: ";
    std::cin >> num_voltas;

    // Usuário faz uma aposta
    std::cout << "Em qual corredor você aposta (1 a " << num_corredores << ")? ";
    std::cin >> aposta;

    while (aposta < 1 || aposta > num_corredores) {
        std::cout << "Aposta inválida! Escolha um corredor entre 1 e " << num_corredores << ": ";
        std::cin >> aposta;
    }

    std::vector<std::thread> threads;

    // Cria uma thread para cada corredor
    for (int i = 0; i < num_corredores; ++i) {
        threads.emplace_back(corredor, i + 1, num_voltas);
    }

    for (int volta = 1; volta <= num_voltas; ++volta) {
        // Inicia a volta
        std::cout << "\nPreparar para a volta " << volta << "...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Apontar...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Já!\n";

        // Libera todos os corredores para a volta atual
        {
            std::lock_guard<std::mutex> lock(mtx);
            ready = true;
        }
        cv.notify_all();

        // Não resetar o sinal aqui para a próxima volta até todos os corredores completarem a volta
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Apenas uma pequena pausa entre as voltas

        // Exibe a tabela de progresso a cada volta
        exibir_tabela(num_corredores, num_voltas);
    }

    // Aguarda todas as threads terminarem
    for (auto &t : threads) {
        t.join();
    }

    // Exibe o vencedor
    int vencedor = -1;
    int menor_tempo = INT_MAX;

    // Calcula o vencedor somando os tempos de todas as voltas
    for (const auto &[id, tempos] : tempos_voltas) {
        int total_time = 0;
        for (int tempo : tempos) {
            total_time += tempo;
        }
        if (total_time < menor_tempo) {
            menor_tempo = total_time;
            vencedor = id;
        }
    }

    // Exibe o resultado
    std::cout << "\nCorrida encerrada!\n";
    std::cout << "O vencedor é o corredor " << vencedor << " com um total de " << menor_tempo << " segundos!\n";

    // Verifica se o usuário ganhou a aposta
    if (aposta == vencedor) {
        std::cout << "Parabéns! Você ganhou a aposta!\n";
    } else {
        std::cout << "Que pena! Você perdeu a aposta.\n";
    }

    return 0;
}
