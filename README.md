# 🧮 Calculadora Modular Deluxe 2.0 (C)

Uma calculadora de linha de comando **completa e modular**, escrita em **C puro**, com mais de **20 funções matemáticas**, suporte a **arrays**, **matrizes**, **histórico de operações**, e **salvamento automático em CSV**.

> Projeto educacional e funcional — demonstra boas práticas de modularização, validação de entrada, manipulação de arquivos e uso de bibliotecas padrão em C (`stdio`, `stdlib`, `string`, `math`, `time`).

---

## 🚀 Funcionalidades

### 🔢 Operações básicas
- Soma, subtração, multiplicação e divisão (com checagem de divisão por zero)
- Potência e raiz *n*-ésima (com verificação de validade)
- Fatorial com limite de segurança (`<= 20`)

### 📊 Estatísticas com vetores
- Média, mediana e desvio-padrão
- Valor máximo e mínimo

### ➗ Matemática discreta
- MDC (máximo divisor comum)
- MMC (mínimo múltiplo comum)

### 🧠 Funções avançadas
- Logaritmo natural (ln)
- Conversões entre **graus ↔ radianos**
- Funções trigonométricas (`sin`, `cos`, `tan`) com verificação de erros

### 🧮 Matrizes 2x2
- Soma e multiplicação matricial (2x2)
- Impressão formatada de matrizes

### 🕒 Histórico e persistência
- Cada operação é registrada em um **histórico em memória**
- Histórico pode ser **listado na tela**
- Salvamento e carregamento automático em **CSV (`historico.csv`)**
- Máximo de **100 operações armazenadas** (FIFO)

---

## 📂 Estrutura do Código

O código está **totalmente modularizado**, com funções agrupadas por tipo:

| Categoria | Funções principais |
|------------|--------------------|
| Entrada/Saída | `ler_inteiro`, `ler_double`, `pausar`, `limpar_buffer` |
| Operações matemáticas | `soma`, `subtracao`, `divisao`, `potencia`, `raiz`, `fatorial` |
| Estatísticas | `media`, `mediana`, `desvio_padrao`, `maximo`, `minimo` |
| Discretas | `mdc`, `mmc` |
| Trigonometria | `trig_sin`, `trig_cos`, `trig_tan` |
| Conversões | `graus_para_radianos`, `radianos_para_graus` |
| Matrizes 2x2 | `soma_matriz_2x2`, `multiplica_matriz_2x2`, `imprimir_matriz_2x2`, `ler_matriz_2x2` |
| Histórico | `adicionar_historico`, `listar_historico`, `salvar_historico_csv`, `carregar_historico_csv` |

---

## 💾 Histórico de Operações

Cada operação é armazenada como uma `struct Operacao`:

```c
typedef struct {
    char tipo[50];     // Tipo da operação (ex: "SOMA", "DIVISAO", "MDC_MMC")
    double a, b;       // Operandos
    double resultado;  // Resultado da operação
    int id;            // ID incremental único
} Operacao;
O histórico é salvo automaticamente ao sair do programa no arquivo:


historico.csv
Exemplo de arquivo CSV gerado:

csv
Copiar código
id,tipo,a,b,resultado
1,SOMA,5,7,12
2,POTENCIA,2,3,8
3,LOG,10,0,2.302585093
🖥️ Uso
## 🧱 Compilação
Use GCC (ou outro compilador C compatível):

bash

gcc calculadora.c -o calculadora -lm
⚠️ A flag -lm é necessária para linkar a biblioteca math.h.

## ▶️ Execução
bash

./calculadora
Ao iniciar, o programa tenta carregar automaticamente o historico.csv, se existir.

## 🧭 Menu principal
pgsql

==== 𝖈𝖆𝖑𝖈𝖚𝖑𝖆𝖉𝖔𝖗𝖆DELUXE2.0 ====
1) Soma
2) Subtracao
3) Multiplicacao
4) Divisao
5) Potencia
6) Raiz
7) Fatorial
8) Media (array)
9) Mediana (array)
10) Desvio-padrao (array)
11) Maximo/Minimo (array)
12) MMC/MDC
13) Log natural
14) Trigonometria (sin/cos/tan)
15) Conversoes grau<->rad
16) Matriz 2x2 (soma/multiplicacao)
17) Historico (listar)
18) Salvar historico em CSV
0) Sair

## 🧩 Exemplo de Execução
makefile

==== 𝖈𝖆𝖑𝖈𝖚𝖑𝖆𝖉𝖔𝖗𝖆DELUXE2.0 ====
1) Soma
...
Escolha uma opcao: 1
A = 10
B = 5
Resultado: 15
FIote, pra continuar clica no ENTER aí...
E ao listar o histórico:

nginx

ID   TIPO        OPERANDOS       RESULTADO
1    SOMA        10, 5           15

## 💡 Destaques Técnicos
Uso seguro de fgets e sscanf (sem scanf inseguro)

Validação robusta de entrada numérica

Organização clara em funções pequenas e reutilizáveis

Persistência simples via CSV

Tratamento de erros matemáticos e divisão por zero

Exemplo prático de manipulação de structs e arrays em C

## 🧠 prováveis upgrades futuros(pedi que a IA me indicasse)
Suporte a matrizes NxN genéricas

Histórico com timestamps (time.h)

Interface gráfica (GTK, ncurses, etc.)

Exportar histórico em JSON

📜 Licença
projeto educativo sem fins lucrativos, apenas com utilidade em desenvolvimento de aprendizagem durante o curso.

Autor: [Ricardo Misael]
📅 calculadora premium super avantajada, remasterizada pro MAX 2.0
💬 “FIote, pra continuar clica no ENTER aí...”
