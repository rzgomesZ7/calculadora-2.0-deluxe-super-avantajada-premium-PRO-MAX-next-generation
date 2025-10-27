// calculadora.c - Calculadora modular em C (20+ funções, arrays, struct, histórico, persistência)

#include <stdio.h>      // preciso para input/output (printf, scanf, FILE, fopen...),resumindo, biblioteca de entrada e saida
#include <stdlib.h>     // preciso para malloc, free e exit
#include <string.h>     // preciso para manipulação de strings (strcpy, strcmp...)
#include <math.h>       // preciso para funções matemáticas (pow, sin, cos, log...)
#include <time.h>       // incluído caso queira timestamps/ids (opcional)

/* Definições de constantes usadas no programa */
// tamanho máximo do histórico que guardamos em memória
#define MAX_HIST 100
// tamanho máximo para ler uma linha do stdin com segurança
#define MAX_LINE 256
// limite seguro para calcular fatorial sem estourar em unsigned long long
#define FACT_LIMIT 20

// Struct para armazenar cada operação no histórico
typedef struct {
    char tipo[50];     // tipo da operação, ex: "SOMA", "POTENCIA",obs.: [isso é um array]
    double a, b;       // operandos usados (b pode ficar 0 se não for usado)
    double resultado;  // resultado da operação (pode ser NAN em erros)
    int id;            // id único incremental da operação
} Operacao;

/* Protótipos das funções organizadas por grupo */

// Funções de entrada/saída
int ler_inteiro(char *prompt);       // lê um inteiro do usuário com validação
double ler_double(char *prompt);     // lê um double do usuário com validação
void limpar_buffer();                // limpa buffer do stdin até '\n'
void pausar();                       // pausa e espera ENTER para continuar

// Funções de cálculo (muitas implementadas)
double soma(double a, double b);                       // soma
double subtracao(double a, double b);                  // subtração
double multiplicacao(double a, double b);              // multiplicação
double divisao(double a, double b, int *erro);         // divisão com checagem de divisão por zero
double potencia(double a, double b);                   // potência a^b
double raiz(double a, double b, int *erro);            // raiz b-ésima de a, com checagens
unsigned long long fatorial(int n, int *erro);         // fatorial (inteiro) com limite
double media(double arr[], int n);                     // média aritmética de um array
double mediana(double arr[], int n);                   // mediana de um array
double desvio_padrao(double arr[], int n);             // desvio padrão populacional
double maximo(double arr[], int n);                    // máximo do array
double minimo(double arr[], int n);                    // mínimo do array
long long mdc(long long a, long long b);               // máximo divisor comum (gcd)
long long mmc(long long a, long long b);               // mínimo múltiplo comum (lcm)
double meu_log(double a, int *erro);                   // log natural com verificação
double graus_para_radianos(double g);                  // converte graus -> rad
double radianos_para_graus(double r);                  // converte rad -> graus

// Funções trigonométricas encapsuladas
double trig_sin(double x);
double trig_cos(double x);
double trig_tan(double x, int *erro);

// Operações com matrizes 2x2
void soma_matriz_2x2(double A[2][2], double B[2][2], double R[2][2]); // soma A + B (2x2)
void multiplica_matriz_2x2(double A[2][2], double B[2][2], double R[2][2]); // produto A * B (2x2)

// Histórico e persistência
void adicionar_historico(Operacao hist[], int *count, Operacao op);   // adiciona operação ao histórico
void listar_historico(Operacao hist[], int count);                    // imprime o histórico
void salvar_historico_csv(Operacao hist[], int count, const char *nome_arquivo); // salva em CSV
int carregar_historico_csv(Operacao hist[], int *count, const char *nome_arquivo); // carrega CSV

// Auxiliares para matrizes
void imprimir_matriz_2x2(double M[2][2]);                              // exibe matriz 2x2 formatada
void ler_matriz_2x2(double M[2][2], char *nome);                       // lê 4 valores para matriz 2x2

/* Implementação das funções */

// limpar_buffer: consome tudo até encontrar '\n' ou EOF (útil após fgets/getchar)
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) ;
}

// pausar: imprime mensagem e espera o usuário pressionar ENTER
void pausar() {
    printf("FIote, pra continuar clica no ENTER aí...");
    fflush(stdout);
    limpar_buffer();
}

// ler_inteiro: lê uma linha e tenta converter para inteiro, repete até sucesso
int ler_inteiro(char *prompt) {
    char linha[MAX_LINE];
    int valor;
    while (1) {
        printf("%s", prompt);
        if (!fgets(linha, sizeof(linha), stdin)) {
            // se fgets falhar por algum motivo, limpamos o estado e tentamos de novo
            clearerr(stdin);
            continue;
        }
        // usamos sscanf para tentar extrair um inteiro
        if (sscanf(linha, "%d", &valor) == 1) {
            return valor; // leitura válida
        } else {
            printf("Tá errado fiote. Digite um número inteiro.\n");
        }
    }
}

// ler_double: lê uma linha e tenta converter para double, repete até sucesso
double ler_double(char *prompt) {
    char linha[MAX_LINE];
    double valor;
    while (1) {
        printf("%s", prompt);
        if (!fgets(linha, sizeof(linha), stdin)) {
            clearerr(stdin);
            continue;
        }
        // tentamos ler um double com sscanf
        if (sscanf(linha, "%lf", &valor) == 1) {
            return valor;
        } else {
            printf("Tá errado fiote. Digite um número (ex: 3.14).\n");
        }
    }
}

/* Funções matemáticas básicas */

// soma: retorna a + b
double soma(double a, double b) { return a + b; }

// subtracao: retorna a - b
double subtracao(double a, double b) { return a - b; }

// multiplicacao: retorna a * b
double multiplicacao(double a, double b) { return a * b; }

// divisao: checa divisão por zero (erro=1) e retorna 0 nesse caso
double divisao(double a, double b, int *erro) {
    if (b == 0.0) {
        *erro = 1; // sinaliza erro para o chamador
        return 0.0;
    }
    *erro = 0;
    return a / b;
}

// potencia: usa pow da math.h para calcular a^b
double potencia(double a, double b) { return pow(a, b); }

// raiz: calcula a^(1/b) com checagens básicas (b != 0 e raiz real quando aplicável)
double raiz(double a, double b, int *erro) {
    if (b == 0.0) {
        *erro = 1; // ordem zero não faz sentido
        return 0.0;
    }
    // se a < 0 e b é par, raiz real não existe
    if (a < 0.0 && fmod(b, 2.0) == 0.0) {
        *erro = 1;
        return 0.0;
    }
    *erro = 0;
    return pow(a, 1.0 / b);
}

// fatorial: calcula n! para n >= 0 e <= FACT_LIMIT, sinaliza erro caso contrário
unsigned long long fatorial(int n, int *erro) {
    if (n < 0) {
        *erro = 1; // fatorial de número negativo não é definido aqui
        return 0ULL;
    }
    if (n > FACT_LIMIT) {
        *erro = 1; // limitamos para evitar overflow em unsigned long long
        return 0ULL;
    }
    *erro = 0;
    unsigned long long res = 1ULL;
    for (int i = 2; i <= n; ++i) res *= (unsigned long long)i;
    return res;
}

// media: soma todos os elementos e divide por n (protege n <= 0)
double media(double arr[], int n) {
    if (n <= 0) return 0.0;
    double s = 0.0;
    for (int i = 0; i < n; ++i) s += arr[i];
    return s / (double)n;
}

// função auxiliar de comparação usada pelo qsort para doubles
int cmp_double(const void *p1, const void *p2) {
    double a = *(double *)p1;
    double b = *(double *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

// mediana: faz uma cópia do array, ordena e retorna a mediana (não altera o original)
double mediana(double arr[], int n) {
    if (n <= 0) return 0.0;
    double *copia = malloc(sizeof(double) * n);
    if (!copia) return 0.0; // se não tem memória, retornamos 0
    for (int i = 0; i < n; ++i) copia[i] = arr[i];
    qsort(copia, n, sizeof(double), cmp_double);
    double med;
    if (n % 2 == 1) med = copia[n / 2];
    else med = (copia[n / 2 - 1] + copia[n / 2]) / 2.0;
    free(copia);
    return med;
}

// desvio_padrao: calcula o desvio padrão populacional (dividindo por n)
double desvio_padrao(double arr[], int n) {
    if (n <= 0) return 0.0;
    double m = media(arr, n);
    double soma = 0.0;
    for (int i = 0; i < n; ++i) {
        double d = arr[i] - m;
        soma += d * d;
    }
    return sqrt(soma / (double)n);
}

// maximo: retorna o maior elemento do array (protege n <= 0)
double maximo(double arr[], int n) {
    if (n <= 0) return 0.0;
    double m = arr[0];
    for (int i = 1; i < n; ++i) if (arr[i] > m) m = arr[i];
    return m;
}

// minimo: retorna o menor elemento do array (protege n <= 0)
double minimo(double arr[], int n) {
    if (n <= 0) return 0.0;
    double m = arr[0];
    for (int i = 1; i < n; ++i) if (arr[i] < m) m = arr[i];
    return m;
}

// mdc: implementa o algoritmo de Euclides, trabalhando com valores absolutos
long long mdc(long long a, long long b) {
    a = llabs(a);
    b = llabs(b);
    if (a == 0) return b;
    if (b == 0) return a;
    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// mmc: calcula mínimo múltiplo usando mmc(a,b) = abs(a/gcd(a,b) * b)
// aqui fazemos a divisão antes para reduzir chance de overflow
long long mmc(long long a, long long b) {
    if (a == 0 || b == 0) return 0; // por praticidade retornamos 0 se algum for 0
    long long g = mdc(a, b);
    return llabs(a / g * b);
}

// meu_log: calcula log natural e sinaliza erro se a <= 0
double meu_log(double a, int *erro) {
    if (a <= 0.0) { *erro = 1; return 0.0; }
    *erro = 0;
    return log(a);
}

// conversões entre graus e radianos (fórmulas padrões)
double graus_para_radianos(double g) { return g * M_PI / 180.0; }
double radianos_para_graus(double r) { return r * 180.0 / M_PI; }

// funções trigonométricas simples que usam as funções da math.h
double trig_sin(double x) { return sin(x); }
double trig_cos(double x) { return cos(x); }

// trig_tan: checa se cos(x) é muito próximo de zero para evitar tangentes indefinidas
double trig_tan(double x, int *erro) {
    double c = cos(x);
    if (fabs(c) < 1e-12) { *erro = 1; return 0.0; }
    *erro = 0;
    return tan(x);
}

/* Operações com matrizes 2x2 */

// soma_matriz_2x2: R = A + B (elemento a elemento)
void soma_matriz_2x2(double A[2][2], double B[2][2], double R[2][2]) {
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            R[i][j] = A[i][j] + B[i][j];
}

// multiplica_matriz_2x2: R = A * B (produto matricial padrão 2x2)
void multiplica_matriz_2x2(double A[2][2], double B[2][2], double R[2][2]) {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            R[i][j] = 0.0;
            for (int k = 0; k < 2; ++k)
                R[i][j] += A[i][k] * B[k][j];
        }
    }
}

// imprimir_matriz_2x2: imprime a matriz com formatação para leitura fácil
void imprimir_matriz_2x2(double M[2][2]) {
    for (int i = 0; i < 2; ++i) {
        printf("| ");
        for (int j = 0; j < 2; ++j) printf("%10.4f ", M[i][j]); // 4 casas
        printf("|\n");
    }
}

// ler_matriz_2x2: pede 4 valores ao usuário e preenche a matriz M
void ler_matriz_2x2(double M[2][2], char *nome) {
    char prompt[80];
    printf("Lendo matriz %s (2x2):\n", nome);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            sprintf(prompt, "Elemento [%d][%d]: ", i, j);
            M[i][j] = ler_double(prompt);
        }
    }
}

/* Histórico (armazenamento em memória e persistência simples em CSV) */

// adicionar_historico: insere op no array hist; se cheio, desloca (FIFO)
void adicionar_historico(Operacao hist[], int *count, Operacao op) {
    if (*count < MAX_HIST) {
        hist[*count] = op;
        (*count)++;
    } else {
        // se já chegou no máximo, descartamos o mais antigo e empurramos os outros
        for (int i = 1; i < MAX_HIST; ++i) hist[i - 1] = hist[i];
        hist[MAX_HIST - 1] = op;
    }
}

// listar_historico: imprime o histórico no formato simples (ID, TIPO, OPERANDOS, RESULTADO)
void listar_historico(Operacao hist[], int count) {
    if (count == 0) {
        printf("Historico vazio,igual os sentimentos de uma IA hehe.\n");
        return;
    }
    printf("ID\tTIPO\t\tOPERANDOS\tRESULTADO\n");
    for (int i = 0; i < count; ++i) {
        // usamos formatos para deixar tabela legível
        printf("%d\t%-10s\t%.6g, %.6g\t%.10g\n",
               hist[i].id, hist[i].tipo, hist[i].a, hist[i].b, hist[i].resultado);
    }
}

// salvar_historico_csv: grava o histórico em um arquivo CSV com cabeçalho
void salvar_historico_csv(Operacao hist[], int count, const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "w");
    if (!f) {
        printf("Erro ao abrir arquivo para salvar.\n");
        return;
    }
    fprintf(f, "id,tipo,a,b,resultado\n");
    for (int i = 0; i < count; ++i) {
        // escrevemos com precisão suficiente para doubles
        fprintf(f, "%d,%s,%.15g,%.15g,%.15g\n",
                hist[i].id, hist[i].tipo, hist[i].a, hist[i].b, hist[i].resultado);
    }
    fclose(f);
    printf("Historico salvo em '%s'\n", nome_arquivo);
}

// carregar_historico_csv: tenta abrir e ler o CSV, retorna 1 se leu ok, 0 se falhou
int carregar_historico_csv(Operacao hist[], int *count, const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return 0; // se não existe arquivo, retornamos 0 (sem erro grave)
    char linha[MAX_LINE];
    int idx = 0;
    // lemos e descartamos o cabeçalho (esperamos que exista)
    if (!fgets(linha, sizeof(linha), f)) { fclose(f); return 0; }
    while (fgets(linha, sizeof(linha), f) && idx < MAX_HIST) {
        Operacao op;
        // inicializamos campos antes do parsing
        op.id = 0; op.tipo[0] = '\0'; op.a = op.b = op.resultado = 0.0;
        // fazemos um parsing simples; aceitaremos quando sscanf conseguir ao menos os primeiros itens
        if (sscanf(linha, "%d,%31[^,],%lf,%lf,%lf", &op.id, op.tipo, &op.a, &op.b, &op.resultado) >= 4) {
            hist[idx++] = op;
        }
    }
    fclose(f);
    *count = idx;
    return 1;
}

/* Função principal: menu interativo que chama todas as funcionalidades */
int main() {
    Operacao historico[MAX_HIST]; // buffer de histórico em memória
    int hist_count = 0;           // quantas operações temos no histórico
    int proximo_id = 1;           // id incremental para atribuir às operações

    // tentamos carregar historico.csv automaticamente (se existir)
    if (carregar_historico_csv(historico, &hist_count, "historico.csv")) {
        if (hist_count > 0) {
            // se carregou, ajustamos o próximo id para não colidir
            proximo_id = historico[hist_count - 1].id + 1;
            printf("Historico carregado (%d itens).\n", hist_count);
        }
    }

    // loop principal do menu; o programa roda até o usuário escolher sair
    while (1) {
        printf("\n==== 𝖈𝖆𝖑𝖈𝖚𝖑𝖆𝖉𝖔𝖗𝖆DELUXE2.0 ====\n");
        printf("1) Soma\n");
        printf("2) Subtracao\n");
        printf("3) Multiplicacao\n");
        printf("4) Divisao\n");
        printf("5) Potencia\n");
        printf("6) Raiz\n");
        printf("7) Fatorial\n");
        printf("8) Media (array)\n");
        printf("9) Mediana (array)\n");
        printf("10) Desvio-padrao (array)\n");
        printf("11) Maximo/Minimo (array)\n");
        printf("12) MMC/MDC\n");
        printf("13) Log natural\n");
        printf("14) Trigonometria (sin/cos/tan)\n");
        printf("15) Conversoes grau<->rad\n");
        printf("16) Matriz 2x2 (soma/multiplicacao)\n");
        printf("17) Historico (listar)\n");
        printf("18) Salvar historico em CSV (historico.csv)\n");
        printf("0) Sair\n");

        int opc = ler_inteiro("Escolha uma opcao: ");

        if (opc == 0) {
            // antes de sair, salvamos o histórico para persistência
            printf("Saindo...\n");
            salvar_historico_csv(historico, hist_count, "historico.csv");
            break;
        }

        // variáveis temporárias usadas nas várias operações
        double a, b, res;
        int erro;
        char tipo_op[32];

        switch (opc) {
            case 1: // soma
                a = ler_double("A = ");
                b = ler_double("B = ");
                res = soma(a, b);
                printf("Resultado: %.10g\n", res);
                snprintf(tipo_op, sizeof(tipo_op), "SOMA");
                {
                    Operacao op1 = {0};
                    strcpy(op1.tipo, tipo_op);
                    op1.a = a; op1.b = b; op1.resultado = res; op1.id = proximo_id++;
                    adicionar_historico(historico, &hist_count, op1);
                }
                pausar();
                break;

            case 2: // subtração
                a = ler_double("A = ");
                b = ler_double("B = ");
                res = subtracao(a, b);
                printf("Resultado: %.10g\n", res);
                snprintf(tipo_op, sizeof(tipo_op), "SUBTRACAO");
                {
                    Operacao op2 = {0};
                    strcpy(op2.tipo, tipo_op);
                    op2.a = a; op2.b = b; op2.resultado = res; op2.id = proximo_id++;
                    adicionar_historico(historico, &hist_count, op2);
                }
                pausar();
                break;

            case 3: // multiplicação
                a = ler_double("A = ");
                b = ler_double("B = ");
                res = multiplicacao(a, b);
                printf("Resultado: %.10g\n", res);
                snprintf(tipo_op, sizeof(tipo_op), "MULTIPLICACAO");
                {
                    Operacao op3 = {0};
                    strcpy(op3.tipo, tipo_op);
                    op3.a = a; op3.b = b; op3.resultado = res; op3.id = proximo_id++;
                    adicionar_historico(historico, &hist_count, op3);
                }
                pausar();
                break;

            case 4: // divisão com verificação de divisão por zero
                a = ler_double("A = ");
                b = ler_double("B = ");
                res = divisao(a, b, &erro);
                if (erro) printf("Erro: divisao por zero!\n");
                else printf("Resultado: %.10g\n", res);
                snprintf(tipo_op, sizeof(tipo_op), "DIVISAO");
                {
                    Operacao op4 = {0};
                    strcpy(op4.tipo, tipo_op);
                    op4.a = a; op4.b = b; op4.resultado = (erro? NAN: res); op4.id = proximo_id++;
                    adicionar_historico(historico, &hist_count, op4);
                }
                pausar();
                break;

            case 5: // potencia
                a = ler_double("Base (A) = ");
                b = ler_double("Expoente (B) = ");
                res = potencia(a, b);
                printf("Resultado: %.10g\n", res);
                snprintf(tipo_op, sizeof(tipo_op), "POTENCIA");
                {
                    Operacao op5 = {0};
                    strcpy(op5.tipo, tipo_op);
                    op5.a = a; op5.b = b; op5.resultado = res; op5.id = proximo_id++;
                    adicionar_historico(historico, &hist_count, op5);
                }
                pausar();
                break;

            case 6: // raiz
                a = ler_double("Valor (A) = ");
                b = ler_double("Ordem (B) = ");
                res = raiz(a, b, &erro);
                if (erro) printf("Erro: raiz invalida (verifique sinais/ordem).\n");
                else printf("Resultado: %.10g\n", res);
                snprintf(tipo_op, sizeof(tipo_op), "RAIZ");
                {
                    Operacao op6 = {0};
                    strcpy(op6.tipo, tipo_op);
                    op6.a = a; op6.b = b; op6.resultado = (erro? NAN: res); op6.id = proximo_id++;
                    adicionar_historico(historico, &hist_count, op6);
                }
                pausar();
                break;

            case 7: // fatorial
            {
                int n = ler_inteiro("N (inteiro) = ");
                unsigned long long f; int ferr;
                f = fatorial(n, &ferr);
                if (ferr) printf("Erro: fatorial inválido (negativo ou > %d)\n", FACT_LIMIT);
                else printf("%d! = %llu\n", n, f);
                snprintf(tipo_op, sizeof(tipo_op), "FATORIAL");
                Operacao op7 = {0};
                strcpy(op7.tipo, tipo_op);
                op7.a = (double)n; op7.b = 0.0; op7.resultado = (double)f; op7.id = proximo_id++;
                adicionar_historico(historico, &hist_count, op7);
                pausar();
                break;
            }

            case 8: // média de array
            {
                int n = ler_inteiro("Quantos elementos? ");
                if (n <= 0) { printf("Numero invalido.\n"); pausar(); break; }
                double *arr = malloc(sizeof(double) * n); // alocamos dinamicamente o array
                for (int i = 0; i < n; ++i) {
                    char prm[64];
                    sprintf(prm, "Elemento %d: ", i);
                    arr[i] = ler_double(prm);
                }
                res = media(arr, n);
                printf("Media = %.10g\n", res);
                snprintf(tipo_op, sizeof(tipo_op), "MEDIA");
                Operacao op8 = {0};
                strcpy(op8.tipo, tipo_op);
                op8.a = n; op8.b = 0; op8.resultado = res; op8.id = proximo_id++;
                adicionar_historico(historico, &hist_count, op8);
                free(arr);
                pausar();
                break;
            }

            case 9: // mediana
            {
                int n = ler_inteiro("Quantos elementos? ");
                if (n <= 0) { printf("Numero invalido.\n"); pausar(); break; }
                double *arr = malloc(sizeof(double) * n);
                for (int i = 0; i < n; ++i) {
                    char prm[64];
                    sprintf(prm, "Elemento %d: ", i);
                    arr[i] = ler_double(prm);
                }
                res = mediana(arr, n);
                printf("Mediana = %.10g\n", res);
                snprintf(tipo_op, sizeof(tipo_op), "MEDIANA");
                Operacao op9 = {0};
                strcpy(op9.tipo, tipo_op);
                op9.a = n; op9.b = 0; op9.resultado = res; op9.id = proximo_id++;
                adicionar_historico(historico, &hist_count, op9);
                free(arr);
                pausar();
                break;
            }

            case 10: // desvio padrão
            {
                int n = ler_inteiro("Quantos elementos? ");
                if (n <= 0) { printf("Numero invalido.\n"); pausar(); break; }
                double *arr = malloc(sizeof(double) * n);
                for (int i = 0; i < n; ++i) {
                    char prm[64];
                    sprintf(prm, "Elemento %d: ", i);
                    arr[i] = ler_double(prm);
                }
                res = desvio_padrao(arr, n);
                printf("Desvio-padrao = %.10g\n", res);
                snprintf(tipo_op, sizeof(tipo_op), "DESVIO");
                Operacao op10 = {0};
                strcpy(op10.tipo, tipo_op);
                op10.a = n; op10.b = 0; op10.resultado = res; op10.id = proximo_id++;
                adicionar_historico(historico, &hist_count, op10);
                free(arr);
                pausar();
                break;
            }

            case 11: // máximo/minimo
            {
                int n = ler_inteiro("Quantos elementos? ");
                if (n <= 0) { printf("Numero invalido.\n"); pausar(); break; }
                double *arr = malloc(sizeof(double) * n);
                for (int i = 0; i < n; ++i) {
                    char prm[64];
                    sprintf(prm, "Elemento %d: ", i);
                    arr[i] = ler_double(prm);
                }
                double mx = maximo(arr, n);
                double mn = minimo(arr, n);
                printf("Maximo = %.10g, Minimo = %.10g\n", mx, mn);
                snprintf(tipo_op, sizeof(tipo_op), "MAXMIN");
                Operacao op11 = {0};
                strcpy(op11.tipo, tipo_op);
                op11.a = mx; op11.b = mn; op11.resultado = 0.0; op11.id = proximo_id++;
                adicionar_historico(historico, &hist_count, op11);
                free(arr);
                pausar();
                break;
            }

            case 12: // MMC / MDC
            {
                long long x = (long long)ler_inteiro("A (inteiro) = ");
                long long y = (long long)ler_inteiro("B (inteiro) = ");
                long long g = mdc(x, y);
                long long l = mmc(x, y);
                printf("MDC = %lld, MMC = %lld\n", g, l);
                snprintf(tipo_op, sizeof(tipo_op), "MDC_MMC");
                Operacao op12 = {0};
                strcpy(op12.tipo, tipo_op);
                op12.a = (double)x; op12.b = (double)y; op12.resultado = (double)l; op12.id = proximo_id++;
                adicionar_historico(historico, &hist_count, op12);
                pausar();
                break;
            }

            case 13: // log natural
            {
                a = ler_double("Valor A = ");
                res = meu_log(a, &erro);
                if (erro) printf("Erro: log indefinido para valores <= 0.\n");
                else printf("ln(%.10g) = %.10g\n", a, res);
                snprintf(tipo_op, sizeof(tipo_op), "LOG");
                Operacao op13 = {0};
                strcpy(op13.tipo, tipo_op);
                op13.a = a; op13.b = 0; op13.resultado = (erro? NAN: res); op13.id = proximo_id++;
                adicionar_historico(historico, &hist_count, op13);
                pausar();
                break;
            }

            case 14: // trigonometria (o usuário fornece o ângulo em graus)
            {
                printf("1) sin\n2) cos\n3) tan\n");
                int t = ler_inteiro("Escolha: ");
                a = ler_double("Angulo em graus: ");
                double rad = graus_para_radianos(a);
                if (t == 1) {
                    res = trig_sin(rad);
                    printf("sin(%.6g deg) = %.10g\n", a, res);
                    snprintf(tipo_op, sizeof(tipo_op), "SIN");
                } else if (t == 2) {
                    res = trig_cos(rad);
                    printf("cos(%.6g deg) = %.10g\n", a, res);
                    snprintf(tipo_op, sizeof(tipo_op), "COS");
                } else if (t == 3) {
                    res = trig_tan(rad, &erro);
                    if (erro) printf("Erro: tangente indefinida para esse angulo.\n");
                    else printf("tan(%.6g deg) = %.10g\n", a, res);
                    snprintf(tipo_op, sizeof(tipo_op), "TAN");
                } else {
                    printf("Opcao invalida.\n");
                    pausar();
                    break;
                }
                Operacao op14 = {0};
                strcpy(op14.tipo, tipo_op);
                op14.a = a; op14.b = 0; op14.resultado = (erro? NAN: res); op14.id = proximo_id++;
                adicionar_historico(historico, &hist_count, op14);
                pausar();
                break;
            }

            case 15: // conversoes grau <-> rad
            {
                printf("1) graus -> radianos\n2) radianos -> graus\n");
                int t = ler_inteiro("Escolha: ");
                if (t == 1) {
                    a = ler_double("Angulo em graus: ");
                    res = graus_para_radianos(a);
                    printf("%.10g graus = %.10g rad\n", a, res);
                    snprintf(tipo_op, sizeof(tipo_op), "G2R");
                } else if (t == 2) {
                    a = ler_double("Angulo em radianos: ");
                    res = radianos_para_graus(a);
                    printf("%.10g rad = %.10g graus\n", a, res);
                    snprintf(tipo_op, sizeof(tipo_op), "R2G");
                } else {
                    printf("Opcao invalida.\n"); pausar(); break;
                }
                Operacao op15 = {0};
                strcpy(op15.tipo, tipo_op);
                op15.a = a; op15.b = 0; op15.resultado = res; op15.id = proximo_id++;
                adicionar_historico(historico, &hist_count, op15);
                pausar();
                break;
            }

            case 16: // operações com matrizes 2x2 (soma ou multiplicação)
            {
                printf("1) Soma de matrizes 2x2\n2) Multiplicacao de matrizes 2x2\n");
                int t = ler_inteiro("Escolha: ");
                double A[2][2], B[2][2], R[2][2];
                ler_matriz_2x2(A, "A");
                ler_matriz_2x2(B, "B");
                if (t == 1) {
                    soma_matriz_2x2(A, B, R);
                    printf("Resultado da soma:\n"); imprimir_matriz_2x2(R);
                    snprintf(tipo_op, sizeof(tipo_op), "MAT_SOMA");
                } else if (t == 2) {
                    multiplica_matriz_2x2(A, B, R);
                    printf("Resultado da multiplicacao:\n"); imprimir_matriz_2x2(R);
                    snprintf(tipo_op, sizeof(tipo_op), "MAT_MUL");
                } else {
                    printf("Opcao invalida.\n"); pausar(); break;
                }
                Operacao op16 = {0};
                strcpy(op16.tipo, tipo_op);
                op16.a = 0; op16.b = 0; op16.resultado = 0; op16.id = proximo_id++;
                adicionar_historico(historico, &hist_count, op16);
                pausar();
                break;
            }

            case 17: // listar histórico
                listar_historico(historico, hist_count);
                pausar();
                break;

            case 18: // salvar histórico manualmente em CSV
                salvar_historico_csv(historico, hist_count, "historico.csv");
                pausar();
                break;

            default: // mensagem de invalidez
                printf("Opa,algo deu errado ai fiote, tente de novo.\n");
                pausar();
                break;
        } // fim do switch
    } // fim do while principal

    return 0;
} // código executado com sucesso
