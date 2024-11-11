#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LEN 100

void ocultarCursor();
void codificar(char senha[20]);
void reposicionarCursor(int x, int y);
void definirCorTexto(int cor);
void desenharMenu();
bool validarSenha(char senha[20]);
void adicionarRegistro();
void visualizarRegistros();
void alterarRegistros();
void excluirRegistro();
void exibirMenuPrincipal();
void decodificar(char senha[20]);

const int setaBaixo = 80;
const int setaCima = 72;
const int teclaEnter = 13;

int main() {
    ocultarCursor();
    setlocale(LC_CTYPE, "Portuguese");
    exibirMenuPrincipal();
    return 0;
}

void ocultarCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void codificar(char senha[20]) {
    int i = 0;
    while (senha[i]) {
        senha[i] += (i + 1); 
        i++;
    }
}

void decodificar(char senha[20]) {
    int i = 0;
    while (senha[i]) {
        senha[i] -= (i + 1); 
        i++;
    }
}

void reposicionarCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void definirCorTexto(int cor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor);
}

void desenharMenu() {
    definirCorTexto(9);

    for (int i = 16; i <= 64; i++) {
        reposicionarCursor(i, 3); 
        printf("-");
    }
    for (int i = 16; i <= 64; i++) {
        reposicionarCursor(i, 18); 
        printf("-");
    }
    for (int i = 4; i < 18; i++) {
        reposicionarCursor(15, i); 
        printf("|");
        reposicionarCursor(65, i); 
        printf("|");
    }

    reposicionarCursor(15, 3); printf("+");
    reposicionarCursor(65, 3); printf("+");
    reposicionarCursor(15, 18); printf("+");
    reposicionarCursor(65, 18); printf("+");

    definirCorTexto(15);
}

bool validarSenha(char senha[20]) {
    bool cond1, cond2, cond3, cond4, cond5;
    cond1 = cond2 = cond3 = cond4 = cond5 = false;
    int tamanho = strlen(senha);

    if (tamanho >= 8 && tamanho <= 12) cond1 = true;

    for (int i = 0; i < tamanho; i++) {
        if (senha[i] >= 'a' && senha[i] <= 'z') cond2 = true;
        if (senha[i] >= 'A' && senha[i] <= 'Z') cond3 = true;
        if (senha[i] >= '0' && senha[i] <= '9') cond5 = true;
        if ((senha[i] >= 32 && senha[i] <= 47) || 
            (senha[i] >= 58 && senha[i] <= 64) ||
            (senha[i] >= 91 && senha[i] <= 96) || 
            (senha[i] >= 123 && senha[i] <= 126)) {
            cond4 = true; 
        }
    }

    return cond1 && cond2 && cond3 && cond4 && cond5;
}

void adicionarRegistro() {
    char usuario[MAX_LEN], senha[MAX_LEN];
    FILE *arquivo;

    system("cls");
    desenharMenu();
    reposicionarCursor(30, 5);
    definirCorTexto(9);
    printf("-> ADICIONAR REGISTRO <-");

    definirCorTexto(15);
    reposicionarCursor(18, 7);
    printf("Digite o nome de usuário: ");
    gets(usuario);

    while (true) {
        definirCorTexto(15);
        reposicionarCursor(18, 8);
        printf("Digite a senha: ");
        gets(senha);

        if (validarSenha(senha)) {
            break; 
        } else {
            reposicionarCursor(18, 10);
            printf("Senha inválida! Deve ter:\n");
            reposicionarCursor(18, 11);
            printf("- Entre 8 e 12 caracteres\n");
            reposicionarCursor(18, 12);
            printf("- Letra maiúscula e minúscula\n");
            reposicionarCursor(18, 13);
            printf("- Pelo menos um número\n");
            reposicionarCursor(18, 14);
            printf("- Um caractere especial\n");
            reposicionarCursor(18, 15);
            printf("Pressione Enter para tentar novamente");

            getch(); 

            reposicionarCursor(18, 8); printf("                          ");
            reposicionarCursor(18, 10); printf("                          ");
            reposicionarCursor(18, 11); printf("                          ");
            reposicionarCursor(18, 12); printf("                          ");
            reposicionarCursor(18, 13); printf("                          ");
            reposicionarCursor(18, 14); printf("                          ");
            reposicionarCursor(18, 15); printf("                          ");
        }
    }

    codificar(usuario);
    codificar(senha);

    arquivo = fopen("registros.txt", "a");
    if (arquivo == NULL) {
        reposicionarCursor(18, 14);
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fprintf(arquivo, "Usuário: %s | Senha: %s\n", usuario, senha);
    fclose(arquivo);
    definirCorTexto(10);
    reposicionarCursor(18, 14);
    printf("Registro adicionado!\n");

    reposicionarCursor(18, 16);
    printf("Pressione qualquer tecla para voltar");
    getch(); 
}

void visualizarRegistros() {
    char linha[MAX_LEN];
    FILE *arquivo;

    system("cls");
    desenharMenu(); 
    definirCorTexto(9);
    reposicionarCursor(30, 5);
    printf("-> VISUALIZAR REGISTROS <-");
    
    definirCorTexto(15);
    arquivo = fopen("registros.txt", "r");
    if (arquivo == NULL) {
        definirCorTexto(12);
        reposicionarCursor(18, 7);
        printf("Nenhum registro encontrado!\n");
        getch();
        return;
    }

    reposicionarCursor(25, 7);
    printf("Lista de registros (criptografada):\n");
    definirCorTexto(12);
    reposicionarCursor(27,8);
    printf("____________________________");
    definirCorTexto(15);
    int linhaY = 9;
    while (fgets(linha, MAX_LEN, arquivo) != NULL) {
        reposicionarCursor(24, linhaY++);
        printf("%s", linha);
    }

    fclose(arquivo);
    definirCorTexto(12);
    reposicionarCursor(22, 19);
    printf("Pressione qualquer tecla para voltar");
    getch(); 
}

void alterarRegistros() {
    char usuario[MAX_LEN], senhaAtual[MAX_LEN], novaSenha[MAX_LEN];
    char linha[MAX_LEN];
    FILE *arquivo, *temp;
    int encontrado = 0;

    system("cls");
    desenharMenu();
    definirCorTexto(9);
    reposicionarCursor(30, 5);
    printf("-> ALTERAR REGISTRO <-");

    definirCorTexto(15);
    reposicionarCursor(18, 7);
    printf("Digite o nome de usuário: ");
    gets(usuario);

    codificar(usuario);

    arquivo = fopen("registros.txt", "r");
    temp = fopen("temp.txt", "w");
    if (arquivo == NULL || temp == NULL) {
        reposicionarCursor(18, 9);
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    while (fgets(linha, MAX_LEN, arquivo) != NULL) {
        if (strstr(linha, usuario) == NULL) {
            fputs(linha, temp);  
        } else {
            encontrado = 1;

            while (true) {
                definirCorTexto(15);
                reposicionarCursor(18, 8);
                printf("Digite a nova senha: ");
                gets(novaSenha);

                if (validarSenha(novaSenha)) {
                    break; 
                } else {
                    reposicionarCursor(18, 10);
                    printf("Senha inválida! Deve ter:\n");
                    reposicionarCursor(18, 11);
                    printf("- Entre 8 e 12 caracteres\n");
                    reposicionarCursor(18, 12);
                    printf("- Letra maiúscula e minúscula\n");
                    reposicionarCursor(18, 13);
                    printf("- Pelo menos um número\n");
                    reposicionarCursor(18, 14);
                    printf("- Um caractere especial\n");
                    reposicionarCursor(18, 15);
                    printf("Pressione Enter para tentar novamente");

                    getch();

                    reposicionarCursor(18, 8); printf("                          ");
                    reposicionarCursor(18, 10); printf("                          ");
                    reposicionarCursor(18, 11); printf("                          ");
                    reposicionarCursor(18, 12); printf("                          ");
                    reposicionarCursor(18, 13); printf("                          ");
                    reposicionarCursor(18, 14); printf("                          ");
                    reposicionarCursor(18, 15); printf("                          ");
                }
            }

            codificar(novaSenha);
            fprintf(temp, "Usuário: %s | Senha: %s\n", usuario, novaSenha);  
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove("registros.txt");
    rename("temp.txt", "registros.txt");

    reposicionarCursor(18, 9);
    if (encontrado) {
        definirCorTexto(10);
        printf("Registro atualizado com sucesso!\n");
    } else {
        definirCorTexto(12);
        printf("Usuário não encontrado!\n");
    }

    reposicionarCursor(18, 11);
    printf("Pressione qualquer tecla para voltar");
    getch();
}



void excluirRegistro() {
    char usuario[MAX_LEN], linha[MAX_LEN];
    FILE *arquivo, *temp;
    int encontrado = 0;

    system("cls");
    desenharMenu();
    definirCorTexto(9);
    reposicionarCursor(30, 5);
    printf("-> EXCLUIR REGISTRO <-");

    definirCorTexto(15);
    reposicionarCursor(18, 7);
    printf("Digite o nome de usuário a ser removido: ");
    gets(usuario);

    codificar(usuario);

    arquivo = fopen("registros.txt", "r");
    temp = fopen("temp.txt", "w");
    if (arquivo == NULL || temp == NULL) {
        reposicionarCursor(18, 9);
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    while (fgets(linha, MAX_LEN, arquivo) != NULL) {
        if (strstr(linha, usuario) == NULL) { 
            fputs(linha, temp);
        } else {
            encontrado = 1;
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove("registros.txt");
    rename("temp.txt", "registros.txt");

    reposicionarCursor(18, 9);
    if (encontrado) {
        definirCorTexto(10);
        printf("Registro excluído!\n");
    } else {
        definirCorTexto(12);
        printf("Registro não encontrado!\n");
    }

    reposicionarCursor(18, 11);
    printf("Pressione qualquer tecla para voltar");
    getch(); 
}

void exibirMenuPrincipal() {
    int opcao = 0;
    int tecla;
    bool sair = false;

    while (!sair) {
        system("cls");
        desenharMenu();
        definirCorTexto(9);
        reposicionarCursor(30, 5);
        printf("-> MENU PRINCIPAL <-");

        definirCorTexto(opcao == 0 ? 12 : 15);
        reposicionarCursor(30, 7);
        printf("Adicionar Registro");

        definirCorTexto(opcao == 1 ? 12 : 15);
        reposicionarCursor(30, 8);
        printf("Visualizar Registros");

        definirCorTexto(opcao == 2 ? 12 : 15);
        reposicionarCursor(30, 9);
        printf("Alterar Registro");

        definirCorTexto(opcao == 3 ? 12 : 15);
        reposicionarCursor(30, 10);
        printf("Excluir Registro");

        definirCorTexto(opcao == 4 ? 12 : 15);
        reposicionarCursor(30, 11);
        printf("Sair");

        tecla = getch();

        switch (tecla) {
            case setaBaixo:
                opcao = (opcao + 1) % 5;
                break;
            case setaCima:
                opcao = (opcao - 1 + 5) % 5;
                break;
            case teclaEnter:
                switch (opcao) {
                    case 0:
                        adicionarRegistro();
                        break;
                    case 1:
                        visualizarRegistros();
                        break;
                    case 2:
                        alterarRegistros();
                        break;
                    case 3:
                        excluirRegistro();
                        break;
                    case 4:
                        sair = true;
                        break;
                }
                break;
        }
    }
}

