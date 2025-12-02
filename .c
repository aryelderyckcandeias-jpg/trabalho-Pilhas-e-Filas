#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição da estrutura para representar uma peça
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único da peça
} Peca;

// Definição da fila circular para peças futuras (capacidade lógica de 5, mas array maior para flexibilidade)
#define TAM_FILA 10  // Tamanho máximo da fila
Peca fila[TAM_FILA];  // Array para armazenar as peças da fila
int front = 0;        // Índice do início da fila
int rear = 0;         // Índice do final da fila
int count_fila = 0;   // Número de elementos na fila

// Definição da pilha para peças reservadas (capacidade de 3)
#define TAM_PILHA 3  // Capacidade máxima da pilha de reserva
Peca pilha[TAM_PILHA]; // Array para armazenar as peças da pilha
int top = -1;          // Índice do topo da pilha (-1 indica pilha vazia)

// Variável global para gerar IDs únicos
static int next_id = 0;

// Função para gerar uma nova peça automaticamente
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};  // Tipos possíveis de peças
    Peca novaPeca;
    novaPeca.nome = tipos[rand() % 4];  // Seleciona um tipo aleatório
    novaPeca.id = next_id++;             // Atribui um ID único e incrementa
    return novaPeca;
}

// Função para inserir uma peça no final da fila (enqueue)
int enqueue(Peca p) {
    if (count_fila >= TAM_FILA) {
        printf("Erro: Fila cheia. Não é possível inserir nova peça.\n");
        return 0;  // Falha
    }
    fila[rear] = p;
    rear = (rear + 1) % TAM_FILA;  // Avança o rear circularmente
    count_fila++;
    return 1;  // Sucesso
}

// Função para remover uma peça do início da fila (dequeue)
Peca dequeue() {
    Peca p = {'\0', -1};  // Peça vazia para caso de erro
    if (count_fila <= 0) {
        printf("Erro: Fila vazia. Não é possível remover peça.\n");
        return p;  // Falha
    }
    p = fila[front];
    front = (front + 1) % TAM_FILA;  // Avança o front circularmente
    count_fila--;
    return p;  // Retorna a peça removida
}

// Função para empilhar uma peça na pilha de reserva (push)
int push(Peca p) {
    if (top >= TAM_PILHA - 1) {
        printf("Erro: Pilha de reserva cheia. Não é possível reservar peça.\n");
        return 0;  // Falha
    }
    top++;
    pilha[top] = p;
    return 1;  // Sucesso
}

// Função para desempilhar uma peça da pilha de reserva (pop)
Peca pop() {
    Peca p = {'\0', -1};  // Peça vazia para caso de erro
    if (top < 0) {
        printf("Erro: Pilha de reserva vazia. Não é possível usar peça reservada.\n");
        return p;  // Falha
    }
    p = pilha[top];
    top--;
    return p;  // Retorna a peça removida
}

// Função para trocar a peça da frente da fila com o topo da pilha
void trocarPecaAtual() {
    if (count_fila <= 0) {
        printf("Erro: Fila vazia. Não é possível trocar peça.\n");
        return;
    }
    if (top < 0) {
        printf("Erro: Pilha vazia. Não é possível trocar peça.\n");
        return;
    }
    // Troca os valores
    Peca temp = fila[front];
    fila[front] = pilha[top];
    pilha[top] = temp;
    printf("Troca realizada: peça da frente da fila [%c %d] com topo da pilha [%c %d].\n",
           pilha[top].nome, pilha[top].id, fila[front].nome, fila[front].id);
}

// Função para trocar os 3 primeiros da fila com as 3 peças da pilha
void trocaMultipla() {
    if (count_fila < 3) {
        printf("Erro: Fila não tem 3 peças. Não é possível realizar troca múltipla.\n");
        return;
    }
    if (top < 2) {  // top >= 2 significa pelo menos 3 peças (índices 0,1,2)
        printf("Erro: Pilha não tem 3 peças. Não é possível realizar troca múltipla.\n");
        return;
    }
    // Troca as 3 primeiras da fila com as 3 da pilha
    for (int i = 0; i < 3; i++) {
        int idx_fila = (front + i) % TAM_FILA;  // Índice na fila
        int idx_pilha = top - i;                // Índice na pilha (top, top-1, top-2)
        Peca temp = fila[idx_fila];
        fila[idx_fila] = pilha[idx_pilha];
        pilha[idx_pilha] = temp;
    }
    printf("Troca múltipla realizada: 3 primeiros da fila com as 3 da pilha.\n");
}

// Função para exibir o estado atual da fila e da pilha
void exibirEstado() {
    printf("Estado atual:\n");
    
    // Exibe a fila
    printf("Fila de peças\t");
    if (count_fila == 0) {
        printf("(vazia)\n");
    } else {
        for (int i = 0; i < count_fila; i++) {
            int idx = (front + i) % TAM_FILA;  // Calcula o índice circular
            printf("[%c %d] ", fila[idx].nome, fila[idx].id);
        }
        printf("\n");
    }
    
    // Exibe a pilha
    printf("Pilha de reserva\t(Topo -> base): ");
    if (top < 0) {
        printf("(vazia)\n");
    } else {
        for (int i = top; i >= 0; i--) {
            printf("[%c %d] ", pilha[i].nome, pilha[i].id);
        }
        printf("\n");
    }
}

// Função principal
int main() {
    srand(time(NULL));  // Inicializa o gerador de números aleatórios

    // Inicializa a fila com 5 peças
    for (int i = 0; i < 5; i++) {
        Peca p = gerarPeca();
        enqueue(p);
    }

    int opcao;
    do {
        // Exibe o estado atual
        exibirEstado();

        // Exibe o menu de opções
        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {  // Jogar peça: dequeue da fila
                Peca removida = dequeue();
                if (removida.id != -1) {
                    printf("Peça jogada: [%c %d]\n", removida.nome, removida.id);
                    // Adiciona uma nova peça para manter a fila cheia
                    Peca nova = gerarPeca();
                    enqueue(nova);
                }
                break;
            }
            case 2: {  // Reservar peça: move da fila para a pilha
                Peca removida = dequeue();
                if (removida.id != -1) {
                    if (push(removida)) {
                        printf("Peça enviada para reserva: [%c %d]\n", removida.nome, removida.id);
                        // Adiciona uma nova peça para manter a fila cheia
                        Peca nova = gerarPeca();
                        enqueue(nova);
                    } else {
                        // Se não conseguiu empilhar, recoloca na fila
                        enqueue(removida);
                    }
                }
                break;
            }
            case 3: {  // Usar peça reservada: pop da pilha
                Peca usada = pop();
                if (usada.id != -1) {
                    printf("Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                }
                break;
            }
            case 4: {  // Trocar peça atual
                trocarPecaAtual();
                break;
            }
            case 5: {  // Troca múltipla
                trocaMultipla();
                break;
            }
            case 0:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
