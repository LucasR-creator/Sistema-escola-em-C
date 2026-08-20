#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALUNOS 100
#define TAM_STRING 100

struct Aluno {
    int matricula;
    char nome[TAM_STRING];
    int idade;
    char curso[TAM_STRING];
    char email[TAM_STRING];
};

void limpaBufferEntrada() {
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrarAluno(
    struct Aluno alunos[],
    int *total_alunos,
    int *proxima_matricula
) {

    if (*total_alunos >= MAX_ALUNOS) {
        printf("\nLimite de alunos atingido.\n");
        return;
    }

    struct Aluno novo_aluno;

    printf("\n================================\n");
    printf("       CADASTRO DE ALUNO\n");
    printf("================================\n");

    printf("Digite o nome do aluno: ");
    fgets(novo_aluno.nome, TAM_STRING, stdin);
    novo_aluno.nome[strcspn(novo_aluno.nome, "\n")] = '\0';

    printf("Digite a idade do aluno: ");
    scanf("%d", &novo_aluno.idade);
    limpaBufferEntrada();

    printf("Digite o curso do aluno: ");
    fgets(novo_aluno.curso, TAM_STRING, stdin);
    novo_aluno.curso[strcspn(novo_aluno.curso, "\n")] = '\0';

    printf("Digite o email do aluno: ");
    fgets(novo_aluno.email, TAM_STRING, stdin);
    novo_aluno.email[strcspn(novo_aluno.email, "\n")] = '\0';

    /* Gera a matrícula automaticamente */
    novo_aluno.matricula = *proxima_matricula;

    /* Guarda o aluno no vetor */
    alunos[*total_alunos] = novo_aluno;

    /* Aumenta a quantidade de alunos */
    (*total_alunos)++;

    /* Prepara a próxima matrícula */
    (*proxima_matricula)++;

    printf("\n================================\n");
    printf("       ALUNO CADASTRADO!\n");
    printf("================================\n");

    printf("Matricula: %d\n", novo_aluno.matricula);
    printf("Nome: %s\n", novo_aluno.nome);
    printf("Idade: %d\n", novo_aluno.idade);
    printf("Curso: %s\n", novo_aluno.curso);
    printf("Email: %s\n", novo_aluno.email);
}

int main() {

    struct Aluno alunos[MAX_ALUNOS];

    int total_alunos = 0;
    int proxima_matricula = 1001;
    int opcao;

    do {

        printf("\n================================\n");
        printf("         SISTEMA ESCOLAR\n");
        printf("================================\n");
        printf("1 - Cadastrar aluno\n");
        printf("0 - Sair\n");
        printf("================================\n");

        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limpaBufferEntrada();

        switch (opcao) {

            case 1:
                cadastrarAluno(
                    alunos,
                    &total_alunos,
                    &proxima_matricula
                );
                break;

            case 0:
                printf("\nSaindo do sistema...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}