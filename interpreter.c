#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "print.h"
#include "emath.h"

// Implementação das funções.
void execute_print(char* text) {
    if (text) {
        printf("%s\n", text);
    }
}

void execute_emat(char* expression) {
    if (!expression) return;
    int num1, num2;
    char op;

    if (sscanf(expression, "%d%c%d", &num1, &op, &num2) != 3) {
        printf("Erro de sintaxe na expressao matematica: %s\n", expression);
        return;
    }

    switch (op) {
        case '+':
            printf("%d\n", num1 + num2);
            break;
        case '-':
            printf("%d\n", num1 - num2);
            break;
        case '*':
            printf("%d\n", num1 * num2);
            break;
        case '/':
            if (num2 != 0) {
                printf("%d\n", num1 / num2);
            } else {
                printf("Erro: Divisao por zero!\n");
            }
            break;
        default:
            printf("Erro: Operador matematico nao suportado: %c\n", op);
            break;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: ./easyc_interpreter <arquivo.ec>\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo '%s'.\n", argv[1]);
        return 1;
    }

    char line[256];
    char* current_print_text = NULL;
    char* current_emat_expression = NULL;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        // Linhas vazias, comentários ou fechamento de parêntese ')' são ignorados.
        if (strlen(line) == 0 || strncmp(line, "//", 2) == 0 || strcmp(line, ")") == 0) {
            continue;
        }

        // --- Lógica para o comando de impressão (print) ---
        if (strncmp(line, "print=(", 7) == 0) {
            if (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\n")] = 0;
                if (strlen(line) > 0 && strncmp(line, "//", 2) != 0 && strcmp(line, ")") != 0) {
                    current_print_text = strdup(line);
                }
            }
        } else if (strncmp(line, "run", 3) == 0 && current_print_text != NULL) {
            execute_print(current_print_text);
            free(current_print_text);
            current_print_text = NULL;
        }

        // --- Lógica para o comando de matemática (emath) ---
        else if (strncmp(line, "emath=(", 7) == 0) {
            if (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\n")] = 0;
                if (strlen(line) > 0 && strncmp(line, "//", 2) != 0 && strcmp(line, ")") != 0) {
                    current_emat_expression = strdup(line);
                }
            }
        } else if (strncmp(line, "solve", 5) == 0 && current_emat_expression != NULL) {
            execute_emat(current_emat_expression);
            free(current_emat_expression);
            current_emat_expression = NULL;
        }
    }

    fclose(file);
    return 0;
}