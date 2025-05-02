// Alexandre Eiji Tomimura Carvalho - 10371680
// Joao Vitor Rocha Miranda - 10427273

#include <stdio.h>
#include <stdlib.h>

// Estrutura para representar um arquivo
typedef struct Node {
    int valor;
    struct Node* prox;
} Node;

// Função para adicionar um novo nó à lista
void adicionarArquivo(Node** head, int valor) {
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->valor = valor;
    novo->prox = *head;
    *head = novo;
}

// Função recursiva para liberar a lista encadeada
void liberarLista(Node* head) {
    if (head == NULL) return;
    liberarLista(head->prox);
    free(head);
}

// Função recursiva para imprimir os arquivos armazenados
void imprimirLista(Node* head) {
    if (head == NULL) {
        printf("\n");
        return;
    }
    printf("%d ", head->valor);
    imprimirLista(head->prox);
}

// Função auxiliar para mesclar duas listas ordenadas
Node* merge(Node* left, Node* right) {
    if (!left) return right;
    if (!right) return left;

    Node* result = NULL;
    if (left->valor <= right->valor) {
        result = left;
        result->prox = merge(left->prox, right);
    } else {
        result = right;
        result->prox = merge(left, right->prox);
    }
    return result;
}

// Função para dividir a lista ao meio e ordenar com merge sort
void mergeSort(Node** headRef) {
    Node* head = *headRef;
    if (!head || !head->prox) return;

    Node *slow = head, *fast = head->prox;
    while (fast && fast->prox) {
        slow = slow->prox;
        fast = fast->prox->prox;
    }

    Node* mid = slow->prox;
    slow->prox = NULL;

    mergeSort(&head);
    mergeSort(&mid);

    *headRef = merge(head, mid);
}

// Função recursiva para distribuir arquivos entre dois pendrives
void distribuir(int armazenamento, Node* head, Node** arquivosA, Node** arquivosB, int* somaA, int* somaB) {
    int individual = armazenamento / 2;

    // Caso base: lista vazia
    if (head == NULL) {
        return;
    }

    // Verifica se o arquivo pode ser adicionado a algum dos pendrives
    if (*somaA + head->valor <= individual) {
        adicionarArquivo(arquivosA, head->valor);
        *somaA += head->valor;
    } else if (*somaB + head->valor <= individual) {
        adicionarArquivo(arquivosB, head->valor);
        *somaB += head->valor;
    } else {
        printf("Impossível armazenar arquivo de tamanho %d!\n", head->valor);
    }

    // Chamada recursiva para o próximo elemento da lista
    distribuir(armazenamento, head->prox, arquivosA, arquivosB, somaA, somaB);
}

int main() {
    FILE* arquivo = fopen("dados.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    int qtdTestes;
    fscanf(arquivo, "%d", &qtdTestes);
    printf("Quantidade de testes: %d\n", qtdTestes);

    for (int i = 0; i < qtdTestes; i++) {
        int armazenamento, qtdArquivos;
        fscanf(arquivo, "%d %d", &armazenamento, &qtdArquivos);
        printf("\nTeste %d:\n\nMemória disponível: %d GB\nQuantidade de arquivos: %d\n", i + 1, armazenamento, qtdArquivos);

        Node* listaArquivos = NULL;
        for (int j = 0; j < qtdArquivos; j++) {
            int valor;
            fscanf(arquivo, "%d", &valor);
            adicionarArquivo(&listaArquivos, valor);
        }

        // Ordenar a lista antes de distribuir os arquivos
        mergeSort(&listaArquivos);
        printf("Arquivos armazenados (ordenados): ");
        imprimirLista(listaArquivos);

        // Inicializa os pendrives e suas somas
        int somaA = 0, somaB = 0;
        Node* arquivosA = NULL;
        Node* arquivosB = NULL;

        // Distribuir os arquivos entre os pendrives
        distribuir(armazenamento, listaArquivos, &arquivosA, &arquivosB, &somaA, &somaB);

        // Imprimir os arquivos armazenados em cada pendrive
        printf("Pendrive A(%dGB): ",armazenamento/2);
        imprimirLista(arquivosA);
        printf("Pendrive B(%dGB): ",armazenamento/2);
        imprimirLista(arquivosB);

        // Liberar memória
        liberarLista(listaArquivos);
        liberarLista(arquivosA);
        liberarLista(arquivosB);
    }

    fclose(arquivo);
    return 0;
}