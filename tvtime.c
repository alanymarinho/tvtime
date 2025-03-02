#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id; // ID único do usuário
    char *login;
    char *senha;
    char *nome;
    int isAdmin;
} Usuario;

typedef struct {
    char *nome;
    char *genero;
    char *duracao;
    int ano;
} Filme;

typedef struct {
    int userId; // ID do usuário que assistiu o filme
    char *nome;
    char *plataforma;
    int dia, mes, ano;
    int avaliacao;
    char *indicacao;
    int horas; // Horas de duração do filme assistido
} FilmeAssistido;

void removerQuebraLinha(char *str) {
    int tamanho = strlen(str);
    if (tamanho > 0 && str[tamanho - 1] == '\n') {
        str[tamanho - 1] = '\0';
    }
}

// Função para gerar o próximo ID único
int gerarProximoId() {
    FILE *file = fopen("usuarios.txt", "r");
    if (!file) {
        return 1; // Se o arquivo não existir, começa com ID 1
    }

    int ultimoId = 0;
    char linha[100];
    while (fgets(linha, sizeof(linha), file)) {
        Usuario user;
        sscanf(linha, "%d;%[^;];%[^;];%[^;];%d", &user.id, linha, linha, linha, &user.isAdmin);
        if (user.id > ultimoId) {
            ultimoId = user.id;
        }
    }
    fclose(file);
    return ultimoId + 1;
}

// Função para cadastrar usuários
void cadastrarUsuario() {
    FILE *file = fopen("usuarios.txt", "a");
    if (!file) {
        printf("Erro ao abrir o arquivo de usuarios!\n");
        return;
    }

    Usuario user;
    user.id = gerarProximoId(); // Gera o próximo ID único

    char buffer[100]; 

    printf("Login: ");
    fgets(buffer, sizeof(buffer), stdin);
    removerQuebraLinha(buffer);
    user.login = malloc(strlen(buffer) + 1);
    strcpy(user.login, buffer);

    printf("Senha: ");
    fgets(buffer, sizeof(buffer), stdin);
    removerQuebraLinha(buffer);
    user.senha = malloc(strlen(buffer) + 1);
    strcpy(user.senha, buffer);

    printf("Nome: ");
    fgets(buffer, sizeof(buffer), stdin);
    removerQuebraLinha(buffer);
    user.nome = malloc(strlen(buffer) + 1);
    strcpy(user.nome, buffer);

    printf("Admin? (1 - Sim, 0 - Nao): ");
    scanf("%d", &user.isAdmin);
    getchar();

    // Escreve os dados na mesma linha, separados por ";"
    fprintf(file, "%d;%s;%s;%s;%d\n", user.id, user.login, user.senha, user.nome, user.isAdmin);
    fclose(file);

    // Libera a memória alocada
    free(user.login);
    free(user.senha);
    free(user.nome);

    printf("Usuario cadastrado com sucesso! ID: %d\n", user.id);
}

// Função para cadastrar filmes
void cadastrarFilme() {
    FILE *file = fopen("filmes.txt", "a");
    if (!file) {
        printf("Erro ao abrir o arquivo de filmes!\n");
        return;
    }

    Filme filme;
    char buffer[100];

    printf("Nome do filme: ");
    fgets(buffer, sizeof(buffer), stdin);
    removerQuebraLinha(buffer);
    filme.nome = malloc(strlen(buffer) + 1);
    strcpy(filme.nome, buffer);

    printf("Genero: ");
    fgets(buffer, sizeof(buffer), stdin);
    removerQuebraLinha(buffer);
    filme.genero = malloc(strlen(buffer) + 1);
    strcpy(filme.genero, buffer);

    printf("Duracao (em minutos): ");
    fgets(buffer, sizeof(buffer), stdin);
    removerQuebraLinha(buffer);
    filme.duracao = malloc(strlen(buffer) + 1);
    strcpy(filme.duracao, buffer);

    printf("Ano de lancamento: ");
    scanf("%d", &filme.ano);
    getchar();

    // Escreve os dados no arquivo
    fprintf(file, "%s;%s;%s;%d\n", filme.nome, filme.genero, filme.duracao, filme.ano);
    fclose(file);

    // Libera a memória alocada
    free(filme.nome);
    free(filme.genero);
    free(filme.duracao);

    printf("Filme cadastrado com sucesso!\n");
}

// Função para registrar um filme assistido
void registrarFilmeAssistido(int userId) {
    FILE *fileFilmes = fopen("filmes.txt", "r");
    if (!fileFilmes) {
        printf("Erro ao abrir o arquivo de filmes!\n");
        return;
    }

    Filme filme;
    FilmeAssistido fa;
    int index = 1;

    // Aloca memória para os campos da estrutura Filme
    filme.nome = malloc(30);   // Tamanho máximo para o nome
    filme.genero = malloc(40); // Tamanho máximo para o gênero
    filme.duracao = malloc(15); // Tamanho máximo para a duração

    if (!filme.nome || !filme.genero || !filme.duracao) {
        printf("Erro ao alocar memoria!\n");
        fclose(fileFilmes);
        return;
    }

    // Listar filmes cadastrados
    while (fscanf(fileFilmes, "%[^;];%[^;];%[^;];%d\n", filme.nome, filme.genero, filme.duracao, &filme.ano) == 4) {
        printf("[%d] - %s - %s\n", index++, filme.nome, filme.genero);
    }

    // Obter o índice do filme escolhido
    int escolha;
    printf("Escolha o numero do filme assistido: ");
    scanf("%d", &escolha);
    getchar();

    if (escolha < 1 || escolha > index - 1) {
        printf("Indice invalido.\n");
        free(filme.nome);
        free(filme.genero);
        free(filme.duracao);
        fclose(fileFilmes);
        return;
    }

    // Reabrir o arquivo para pegar o nome e a duração do filme selecionado
    rewind(fileFilmes);
    index = 1;
    while (fscanf(fileFilmes, "%[^;];%[^;];%[^;];%d\n", filme.nome, filme.genero, filme.duracao, &filme.ano) == 4) {
        if (index == escolha) {
            fa.nome = malloc(strlen(filme.nome) + 1);
            strcpy(fa.nome, filme.nome);
            fa.horas = atoi(filme.duracao); // Armazena a duração em minutos
            break;
        }
        index++;
    }

    fclose(fileFilmes);

    // Libera a memória alocada para os campos da estrutura Filme
    free(filme.nome);
    free(filme.genero);
    free(filme.duracao);

    // Agora registrar as outras informações do filme assistido
    char buffer[100];

    printf("Plataforma: ");
    fgets(buffer, sizeof(buffer), stdin);
    removerQuebraLinha(buffer);
    fa.plataforma = malloc(strlen(buffer) + 1);
    strcpy(fa.plataforma, buffer);

    char data[15];
    printf("Data (DD/MM/AAAA): ");
    fgets(data, sizeof(data), stdin);
    sscanf(data, "%d/%d/%d", &fa.dia, &fa.mes, &fa.ano);

    printf("Avaliacao (0 a 10): ");
    scanf("%d", &fa.avaliacao);
    getchar();

    printf("Indicacao (Sim/Nao): ");
    fgets(buffer, sizeof(buffer), stdin);
    removerQuebraLinha(buffer);
    fa.indicacao = malloc(strlen(buffer) + 1);
    strcpy(fa.indicacao, buffer);

    // Define o userId do filme assistido
    fa.userId = userId;

    // Salvar o filme assistido
    FILE *fileAssistidos = fopen("assistidos.txt", "a");
    if (!fileAssistidos) {
        printf("Erro ao abrir o arquivo de filmes assistidos!\n");
        return;
    }

    // Grava os dados no arquivo
    fprintf(fileAssistidos, "%d;%s;%s;%02d/%02d/%04d;%d;%s;%d\n",
            fa.userId, fa.nome, fa.plataforma, fa.dia, fa.mes, fa.ano, fa.avaliacao, fa.indicacao, fa.horas);
    fclose(fileAssistidos);

    // Libera a memória alocada
    free(fa.nome);
    free(fa.plataforma);
    free(fa.indicacao);

    printf("Filme assistido registrado com sucesso!\n");
}

void listarFilmesAssistidos(int userId) {
    FILE *file = fopen("assistidos.txt", "r");
    if (!file) {
        printf("Nenhum filme assistido registrado.\n");
        return;
    }

    FilmeAssistido fa;
    char linha[200];

    // Aloca memória para os campos da estrutura FilmeAssistido
    fa.nome = malloc(30);       // Tamanho máximo para o nome
    fa.plataforma = malloc(15); // Tamanho máximo para a plataforma
    fa.indicacao = malloc(3);   // Tamanho máximo para a indicação

    // Verifica se a alocação de memória foi bem-sucedida
    if (!fa.nome || !fa.plataforma || !fa.indicacao) {
        printf("Erro ao alocar memoria!\n");
        // Libera qualquer memória que foi alocada com sucesso
        if (fa.nome) free(fa.nome);
        if (fa.plataforma) free(fa.plataforma);
        if (fa.indicacao) free(fa.indicacao);
        fclose(file);
        return;
    }

    printf("Filmes assistidos:\n");

    while (fgets(linha, sizeof(linha), file)) {
        // Lê os dados da linha
        int result = sscanf(linha, "%d;%[^;];%[^;];%d/%d/%d;%d;%[^;];%d", &fa.userId, fa.nome, fa.plataforma, &fa.dia, &fa.mes, &fa.ano, &fa.avaliacao, fa.indicacao, &fa.horas);

        // Verifica se a leitura foi bem-sucedida
        if (result == 9) {
            // Verifica se o filme foi assistido pelo usuário logado
            if (fa.userId == userId) {
                int horas = fa.horas / 60;
                int minutos = fa.horas % 60;
                printf("%s (Assistido em %02d/%02d/%d na plataforma %s) - Avaliacao: %d/10 - Indicacao: %s - Duracao: %d horas e %d minutos\n",
                       fa.nome, fa.dia, fa.mes, fa.ano, fa.plataforma, fa.avaliacao, fa.indicacao, horas, minutos);
            }
        } else {
            printf("Erro ao ler linha do arquivo: %s\n", linha);
        }
    }

    fclose(file);

    // Libera a memória alocada
    if (fa.nome) free(fa.nome);
    if (fa.plataforma) free(fa.plataforma);
    if (fa.indicacao) free(fa.indicacao);
}

// Função para calcular o total de horas assistidas
void calcularHorasAssistidas(int userId) {
    FILE *file = fopen("assistidos.txt", "r");
    if (!file) {
        printf("Nenhum filme assistido registrado.\n");
        return;
    }

    FilmeAssistido fa;
    char linha[200];
    int totalHoras = 0;

    // Aloca memória para os campos da estrutura FilmeAssistido
    fa.nome = malloc(30);       // Tamanho máximo para o nome
    fa.plataforma = malloc(15); // Tamanho máximo para a plataforma
    fa.indicacao = malloc(3);   // Tamanho máximo para a indicação

    // Verifica se a alocação de memória foi bem-sucedida
    if (!fa.nome || !fa.plataforma || !fa.indicacao) {
        printf("Erro ao alocar memoria!\n");
        // Libera qualquer memória que foi alocada com sucesso
        if (fa.nome) free(fa.nome);
        if (fa.plataforma) free(fa.plataforma);
        if (fa.indicacao) free(fa.indicacao);
        fclose(file);
        return;
    }

    while (fgets(linha, sizeof(linha), file)) {
        // Lê os dados da linha
        int result = sscanf(linha, "%d;%[^;];%[^;];%d/%d/%d;%d;%[^;];%d", &fa.userId, fa.nome, fa.plataforma, &fa.dia, &fa.mes, &fa.ano, &fa.avaliacao, fa.indicacao, &fa.horas);

        // Verifica se a leitura foi bem-sucedida
        if (result == 9) {
            // Verifica se o filme foi assistido pelo usuário logado
            if (fa.userId == userId) {
                totalHoras += fa.horas;
            }
        } else {
            printf("Erro ao ler linha do arquivo: %s\n", linha);
        }
    }

    fclose(file);

    // Libera a memória alocada
    if (fa.nome) free(fa.nome);
    if (fa.plataforma) free(fa.plataforma);
    if (fa.indicacao) free(fa.indicacao);

    printf("Total de tempo assistido: %d horas e %d minutos\n", totalHoras / 60, totalHoras % 60);

}

// Função para realizar o login
int login(Usuario *user) {
    char login[20], senha[10], linha[100];
    FILE *file = fopen("usuarios.txt", "r");
    if (!file) {
        printf("Erro ao abrir o arquivo de usuarios!\n");
        return 0;
    }

    printf("Login: ");
    fgets(login, sizeof(login), stdin);
    removerQuebraLinha(login);

    printf("Senha: ");
    fgets(senha, sizeof(senha), stdin);
    removerQuebraLinha(senha);

    // Lê linha por linha
    while (fgets(linha, sizeof(linha), file)) {
        removerQuebraLinha(linha);

        // Aloca memória para os campos da estrutura
        user->login = malloc(20); // Tamanho máximo para o login
        user->senha = malloc(10); // Tamanho máximo para a senha
        user->nome = malloc(30);  // Tamanho máximo para o nome

        if (!user->login || !user->senha || !user->nome) {
            printf("Erro ao alocar memoria!\n");
            fclose(file);
            return 0;
        }

        // Divide os dados pelo delimitador ";"
        sscanf(linha, "%d;%[^;];%[^;];%[^;];%d", &user->id, user->login, user->senha, user->nome, &user->isAdmin);

        if (strcmp(user->login, login) == 0 && strcmp(user->senha, senha) == 0) {
            fclose(file);
            return 1;  // Login bem-sucedido
        }

        // Libera a memória alocada para evitar vazamentos
        free(user->login);
        free(user->senha);
        free(user->nome);
    }

    fclose(file);
    printf("Login ou senha incorretos.\n");
    return 0;  // Falha no login
}

int main() {
    Usuario user;
    int opcao, loggedIn = 0, isAdmin = 0;

    while (1) {
        if (!loggedIn) {
            printf("----------------------------------\n");
            printf("1. Cadastrar Usuario\n");
            printf("2. Fazer Login\n");
            printf("3. Sair\n");
            printf("----------------------------------\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao);
            getchar();

            switch (opcao) {
                case 1:
                    cadastrarUsuario();
                    break;
                case 2:
                    if (login(&user)) {
                        loggedIn = 1;
                        isAdmin = user.isAdmin;
                        printf("Login bem-sucedido! ID do usuario: %d\n", user.id);
                    }
                    break;
                case 3:
                    printf("Saindo...\n");
                    return 0;
                default:
                    printf("Opcao invalida!\n");
            }
        } else {
            printf("----------------------------------\n");
            if (isAdmin) {
                printf("1. Cadastrar Filme\n");
                printf("2. Sair\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &opcao);
                getchar();

                switch (opcao) {
                    case 1:
                        cadastrarFilme();
                        break;
                    case 2:
                        loggedIn = 0;
                        printf("Desconectado!\n");
                        break;
                    default:
                        printf("Opcao invalida!\n");
                }
            } else {
                printf("1. Registrar Filme Assistido\n");
                printf("2. Listar Filmes Assistidos\n");
                printf("3. Ver Estatisticas de Horas Assistidas\n");
                printf("4. Sair\n");
                printf("----------------------------------\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &opcao);
                getchar();

                switch (opcao) {
                    case 1:
                        registrarFilmeAssistido(user.id);
                        break;
                    case 2:
                        listarFilmesAssistidos(user.id);
                        break;
                    case 3:
                        calcularHorasAssistidas(user.id);
                        break;
                    case 4:
                        loggedIn = 0;
                        printf("Desconectado!\n");
                        break;
                    default:
                        printf("Opcao invalida!\n");
                }
            }
        }
    }
}