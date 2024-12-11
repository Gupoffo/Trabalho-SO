# SSC0140 - Sistemas operacionais I
## Alunos
- Fellipe Tripovichy - 9850332
- Luan Benson Ribeiro - 13672085
- Gustavo de Araújo Poffo - 13672849
---

# Jogo de Corrida Multithreaded

## Descrição

O **Jogo de Corrida Multithreaded** é uma simulação onde múltiplos corredores competem em uma corrida de várias voltas, com tempos aleatórios. O objetivo do jogo é acompanhar o desempenho dos corredores e apostar no vencedor. O uso de **threads** permite que todos os corredores competam simultaneamente, enquanto a **sincronização** é realizada por meio de **semaforos** para garantir que todos comecem as voltas ao mesmo tempo. Ao final da corrida, o jogador pode verificar se acertou sua aposta.

## Como Funciona

1. **Configuração Inicial**:
   - O jogo pede ao jogador para escolher o número de **corredores** e o número de **voltas**.
   - O jogador também faz uma **aposta**, escolhendo o corredor no qual acredita que será o vencedor.

2. **Execução das Voltas**:
   - Cada corredor possui um tempo aleatório para completar cada volta (entre 1 e 3 segundos).
   - Todos os corredores começam as voltas simultaneamente. O jogador precisa pressionar **ENTER** para iniciar cada volta.
   - Durante o jogo, uma **tabela de progresso** é exibida, mostrando o desempenho dos corredores ao longo de cada volta.

3. **Final do Jogo**:
   - Após todas as voltas, o tempo total de cada corredor é calculado.
   - O corredor com o menor tempo total é declarado vencedor.
   - O jogo verifica se o jogador acertou sua aposta no vencedor e exibe uma mensagem de resultado.

## Explicação do Código

O código foi estruturado para simular uma corrida entre múltiplos corredores, utilizando conceitos de **multithreading** (execução paralela) e **sincronização** para garantir que todos os corredores comecem as voltas ao mesmo tempo. Abaixo está a explicação detalhada do funcionamento do código:

### 1. **Bibliotecas Utilizadas**:
   - **`#include <thread>`**: Para criar e gerenciar as threads, que representam os corredores.
   - **`#include <vector>`**: Para armazenar os tempos de cada volta de cada corredor.
   - **`#include <mutex>`**: Usado para garantir a segurança no acesso a dados compartilhados entre threads.
   - **`#include <condition_variable>`**: Para a sincronização das threads, garantindo que todos os corredores comecem as voltas ao mesmo tempo.
   - **`#include <chrono>`**: Para manipulação de tempo, especialmente para simular o tempo de corrida.
   - **`#include <random>`**: Para gerar tempos aleatórios para cada volta dos corredores.
   - **`#include <iomanip>`**: Para formatar a saída da tabela de progresso de forma organizada.

### 2. **Variáveis de Sincronização**:
   - **`std::mutex mtx`**: Utilizado para bloquear seções críticas do código, garantindo que dados compartilhados, como os tempos dos corredores, sejam acessados de forma segura.
   - **`std::condition_variable cv`**: Permite que todas as threads de corredores aguardem até que seja o momento de começar a volta. A sincronização é feita com a variável `ready`.
   - **`ready`**: Controle de estado, indicando se todas as threads estão prontas para iniciar uma volta.

### 3. **Função `corredor()`**:
   - Esta função é executada por cada thread (representando um corredor). A função:
     - Gera um **tempo aleatório** para cada volta do corredor.
     - Espera o sinal para começar a volta (via `std::condition_variable`).
     - Registra o tempo de cada volta no vetor `tempos_voltas`.
     - Após completar todas as voltas, calcula o **tempo total** e exibe o resultado para aquele corredor.

### 4. **Função `exibir_tabela()`**:
   - Exibe uma tabela mostrando o progresso de cada corredor após cada volta. Cada linha representa uma volta, e as colunas indicam o tempo de cada corredor naquela volta ou "Não" se o corredor ainda não completou a volta até aquele momento.
   - A tabela é formatada usando a biblioteca `iomanip` para garantir um bom alinhamento.

### 5. **Função `main()`**:
   - **Entrada do Jogador**: O programa pede ao jogador para inserir o número de corredores e voltas, e também para escolher o corredor no qual deseja apostar.
   - **Criação de Threads**: Para cada corredor, uma **thread** é criada, e cada thread executa a função `corredor()`.
   - **Sincronização das Voltas**: Para cada volta, o programa aguarda o jogador pressionar **ENTER**. Quando isso acontece, a variável `ready` é configurada para `true` e a função `cv.notify_all()` é chamada para liberar todas as threads, permitindo que os corredores comecem a volta simultaneamente.
   - **Aguardar o Término das Threads**: Após o término das voltas, o programa aguarda todas as threads (corredores) completarem suas execuções com `join()`.

### 6. **Determinação do Vencedor**:
   - Após todas as voltas serem concluídas, o código calcula o **tempo total** de cada corredor, e aquele com o menor tempo é declarado vencedor.
   - O programa verifica se o corredor vencedor é o que o jogador apostou e exibe uma mensagem de vitória ou derrota.

## O Uso de Semáforos no Jogo

Os **semáforos** são utilizados aqui através da combinação de **mutexes** e **variáveis de condição** para garantir que todos os corredores (threads) comecem a corrida simultaneamente, ou seja, ao mesmo tempo. O semáforo garante a **sincronização entre as threads**, impedindo que um corredor inicie antes do outro.

- **Mutex**: Protege o acesso a variáveis compartilhadas, como o vetor `tempos_voltas`, evitando que múltiplas threads alterem essas variáveis ao mesmo tempo, o que poderia gerar resultados inconsistentes.
  
- **Variável de Condição (`cv`)**: Permite que as threads de corredores "esperem" até que todas estejam prontas para começar a volta ao mesmo tempo. Elas aguardam até que a variável `ready` seja definida como `true`, momento em que todas as threads são liberadas para executar sua parte da corrida simultaneamente.

## O Uso de Threads no Jogo

As **threads** são fundamentais para simular a execução paralela da corrida. Cada **corredor** é representado por uma **thread** que realiza a função `corredor()`. As threads competem de forma independente, mas ainda precisam estar sincronizadas para garantir que todas comecem as voltas ao mesmo tempo.

- Cada thread é responsável por simular o **tempo de cada volta**, gerando tempos aleatórios e aguardando pela sincronização para começar a corrida.
- O uso de threads permite que os tempos de todos os corredores sejam calculados **paralelamente**, sem a necessidade de aguardar que um corredor termine para o próximo começar, o que torna o processo mais eficiente e realista.

## Conclusão

O **Jogo de Corrida Multithreaded** é uma excelente implementação para demonstrar o uso de **programação concorrente**, utilizando **threads** e **sincronização** para criar uma simulação eficiente e realista de uma corrida entre múltiplos corredores. O uso de semáforos (como mutexes e variáveis de condição) garante que as threads de corredores sejam sincronizadas e possam competir de maneira justa. Este jogo também fornece uma maneira divertida e prática de ilustrar conceitos de **multithreading**, que são essenciais em muitos sistemas modernos, onde várias operações precisam ser executadas simultaneamente.
