#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALUNOS 100
#define TAM_STRING 100
#define ARQUIVO "alunos.dat"

struct Aluno {
    int matricula;
    char nome[TAM_STRING];
    int idade;
    char curso[TAM_STRING];
    char email[TAM_STRING];
};

/* Agrupa tudo o que pertence ao sistema em uma unica estrutura */
struct Escola {
    struct Aluno alunos[MAX_ALUNOS];
    int total_alunos;
    int proxima_matricula;
};

/* ---------- Entrada de dados com validacao ---------- */

void limpaBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Le um inteiro entre minimo e maximo; repete ate o usuario acertar */
int lerInteiro(const char *mensagem, int minimo, int maximo) {
    int valor;

    while (1) {
        printf("%s", mensagem);
        int resultado = scanf("%d", &valor);

        if (resultado == EOF) {
            printf("\nEntrada encerrada.\n");
            exit(0);
        }
        limpaBufferEntrada();

        if (resultado != 1) {
            printf("Entrada invalida. Digite apenas numeros.\n");
            continue;
        }
        if (valor < minimo || valor > maximo) {
            printf("Digite um valor entre %d e %d.\n", minimo, maximo);
            continue;
        }
        return valor;
    }
}

/* Le um texto nao vazio e remove o '\n' do final */
void lerTexto(const char *mensagem, char *destino, int tamanho) {
    while (1) {
        printf("%s", mensagem);

        if (fgets(destino, tamanho, stdin) == NULL) {
            printf("\nEntrada encerrada.\n");
            exit(0);
        }

        /* Sem '\n' no texto: a linha era maior que o espaco disponivel.
           Descarta o resto para nao atrapalhar a proxima leitura. */
        if (strchr(destino, '\n') == NULL) {
            limpaBufferEntrada();
        }
        destino[strcspn(destino, "\n")] = '\0';

        if (strlen(destino) > 0) {
            return;
        }
        printf("Este campo nao pode ficar vazio.\n");
    }
}

/* Validacao simples: sem espacos, com '@' e com '.' depois do '@' */
int emailValido(const char *email) {
    const char *arroba = strchr(email, '@');

    if (strchr(email, ' ') != NULL) return 0;
    if (arroba == NULL || arroba == email) return 0;
    return strchr(arroba, '.') != NULL;
}

void lerEmail(char *destino, int tamanho) {
    while (1) {
        lerTexto("Digite o email do aluno: ", destino, tamanho);
        if (emailValido(destino)) {
            return;
        }
        printf("Email invalido. Exemplo: nome@dominio.com\n");
    }
}

/* ---------- Persistencia (salvar e carregar em arquivo) ---------- */

void salvarDados(const struct Escola *escola) {
    FILE *arquivo = fopen(ARQUIVO, "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar os dados.\n");
        return;
    }
    fwrite(escola, sizeof(struct Escola), 1, arquivo);
    fclose(arquivo);
}

void carregarDados(struct Escola *escola) {
    struct Escola lida;
    FILE *arquivo;

    memset(escola, 0, sizeof(struct Escola));
    escola->proxima_matricula = 1001;

    arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        return; /* primeira execucao: ainda nao ha arquivo */
    }

    if (fread(&lida, sizeof(struct Escola), 1, arquivo) == 1
        && lida.total_alunos >= 0
        && lida.total_alunos <= MAX_ALUNOS) {
        *escola = lida;
    }
    fclose(arquivo);
}

/* ---------- Funcionalidades ---------- */

void cadastrarAluno(struct Escola *escola) {
    struct Aluno *novo;

    if (escola->total_alunos >= MAX_ALUNOS) {
        printf("\nLimite de alunos atingido.\n");
        return;
    }

    /* Preenche direto na posicao livre do vetor */
    novo = &escola->alunos[escola->total_alunos];

    printf("\n================================\n");
    printf("       CADASTRO DE ALUNO\n");
    printf("================================\n");

    lerTexto("Digite o nome do aluno: ", novo->nome, TAM_STRING);
    novo->idade = lerInteiro("Digite a idade do aluno: ", 1, 120);
    lerTexto("Digite o curso do aluno: ", novo->curso, TAM_STRING);
    lerEmail(novo->email, TAM_STRING);

    novo->matricula = escola->proxima_matricula;
    escola->total_alunos++;
    escola->proxima_matricula++;

    salvarDados(escola);

    printf("\n================================\n");
    printf("       ALUNO CADASTRADO!\n");
    printf("================================\n");
    printf("Matricula: %d\n", novo->matricula);
    printf("Nome: %s\n", novo->nome);
    printf("Idade: %d\n", novo->idade);
    printf("Curso: %s\n", novo->curso);
    printf("Email: %s\n", novo->email);
}

void listarAlunos(const struct Escola *escola) {
    int i;

    printf("\n================================\n");
    printf("       LISTA DE ALUNOS\n");
    printf("================================\n");

    if (escola->total_alunos == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("%-10s %-22s %-6s %-18s %s\n",
           "MATRICULA", "NOME", "IDADE", "CURSO", "EMAIL");

    for (i = 0; i < escola->total_alunos; i++) {
        const struct Aluno *a = &escola->alunos[i];
        printf("%-10d %-22.22s %-6d %-18.18s %s\n",
               a->matricula, a->nome, a->idade, a->curso, a->email);
    }
    printf("\nTotal: %d aluno(s)\n", escola->total_alunos);
}

int main(void) {
    struct Escola escola;
    int opcao;

    carregarDados(&escola);

    do {
        printf("\n================================\n");
        printf("         SISTEMA ESCOLAR\n");
        printf("================================\n");
        printf("1 - Cadastrar aluno\n");
        printf("2 - Listar alunos\n");
        printf("0 - Sair\n");
        printf("================================\n");

        opcao = lerInteiro("Escolha uma opcao: ", 0, 2);

        switch (opcao) {
            case 1:
                cadastrarAluno(&escola);
                break;
            case 2:
                listarAlunos(&escola);
                break;
            case 0:
                printf("\nSaindo do sistema...\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}