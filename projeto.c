#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TAREFAS 100

typedef struct {
    char titulo[100];
    int concluida;   
    int prioridade;  
} Tarefa;

void mostrarBanner() {
    printf("\n==============================\n");
    printf("     GERENCIADOR DE TAREFAS   \n");
    printf("==============================\n\n");
}

void mostrarProgresso(Tarefa tarefas[], int qtd) {
    if (qtd == 0) {
        printf("\nNenhuma tarefa cadastrada ainda.\n");
        return;
    }

    int concluidas = 0;
    for (int i = 0; i < qtd; i++) {
        if (tarefas[i].concluida) concluidas++;
    }

    float perc = (concluidas * 100.0f) / qtd;
    int barras = perc / 10; 

    printf("\nProgresso: [");
    for (int i = 0; i < 10; i++) {
        if (i < barras) printf("#");
        else printf(" ");
    }
    printf("] %.1f%% (%d/%d concluídas)\n", perc, concluidas, qtd);
}

void adicionarTarefa(Tarefa tarefas[], int *qtd) {
    if (*qtd >= MAX_TAREFAS) {
        printf("Limite de tarefas atingido!\n");
        return;
    }

    printf("Digite o titulo da tarefa: ");
    getchar(); 
    fgets(tarefas[*qtd].titulo, 100, stdin);
    tarefas[*qtd].titulo[strcspn(tarefas[*qtd].titulo, "\n")] = 0; 

    printf("Prioridade (1=Alta, 2=Media, 3=Baixa): ");
    scanf("%d", &tarefas[*qtd].prioridade);

    tarefas[*qtd].concluida = 0;
    (*qtd)++;
    printf("Tarefa adicionada com sucesso!\n");
}

void listarTarefas(Tarefa tarefas[], int qtd) {
    if (qtd == 0) {
        printf("Nenhuma tarefa cadastrada.\n");
        return;
    }

    printf("\n=== Lista de Tarefas ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d. [%c] %s (Prioridade: %d)\n", 
               i + 1, 
               tarefas[i].concluida ? 'X' : ' ', 
               tarefas[i].titulo, 
               tarefas[i].prioridade);
    }

    mostrarProgresso(tarefas, qtd);
}

void concluirTarefa(Tarefa tarefas[], int qtd) {
    int id;
    listarTarefas(tarefas, qtd);
    if (qtd == 0) return;

    printf("Digite o numero da tarefa para concluir: ");
    scanf("%d", &id);

    if (id < 1 || id > qtd) {
        printf("Tarefa invalida!\n");
        return;
    }

    tarefas[id - 1].concluida = 1;
    printf("Tarefa concluida com sucesso!\n");
}

void excluirTarefa(Tarefa tarefas[], int *qtd) {
    int id;
    listarTarefas(tarefas, *qtd);
    if (*qtd == 0) return;

    printf("Digite o numero da tarefa para excluir: ");
    scanf("%d", &id);

    if (id < 1 || id > *qtd) {
        printf("Tarefa invalida!\n");
        return;
    }

    for (int i = id - 1; i < *qtd - 1; i++) {
        tarefas[i] = tarefas[i + 1];
    }

    (*qtd)--;
    printf("Tarefa excluida com sucesso!\n");
}

void salvarTarefas(Tarefa tarefas[], int qtd) {
    FILE *f = fopen("tarefas.bin", "wb");
    if (f == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }
    fwrite(&qtd, sizeof(int), 1, f);
    fwrite(tarefas, sizeof(Tarefa), qtd, f);
    fclose(f);
    printf("Tarefas salvas em arquivo!\n");
}

void carregarTarefas(Tarefa tarefas[], int *qtd) {
    FILE *f = fopen("tarefas.bin", "rb");
    if (f == NULL) {
        printf("Nenhum arquivo encontrado.\n");
        return;
    }
    fread(qtd, sizeof(int), 1, f);
    fread(tarefas, sizeof(Tarefa), *qtd, f);
    fclose(f);
    printf("Tarefas carregadas do arquivo!\n");
}

int main() {
    Tarefa tarefas[MAX_TAREFAS];
    int qtd = 0;
    int opcao;

    do {
        mostrarBanner();
        printf("1 - Adicionar tarefa\n");
        printf("2 - Listar tarefas\n");
        printf("3 - Concluir tarefa\n");
        printf("4 - Excluir tarefa\n");
        printf("5 - Salvar tarefas\n");
        printf("6 - Carregar tarefas\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: adicionarTarefa(tarefas, &qtd); break;
            case 2: listarTarefas(tarefas, qtd); break;
            case 3: concluirTarefa(tarefas, qtd); break;
            case 4: excluirTarefa(tarefas, &qtd); break;
            case 5: salvarTarefas(tarefas, qtd); break;
            case 6: carregarTarefas(tarefas, &qtd); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while(opcao != 0);

    return 0;
}
