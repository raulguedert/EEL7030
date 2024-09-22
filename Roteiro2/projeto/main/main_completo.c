// Código com os exercícios resolvidos
// Exercício 3: Substituir 0x100 por 0x4 ou (1 << 2)
// Exercício 4: Substituir 0x100 por 0x700 ou ((1 << 10) + (1 << 9) + (1 << 8))

// Bibliotecas
#include "soc/soc.h" // Biblioteca com o macro REG_WRITE

// Definições
#define GPIO_ENABLE_REG 0x60004020
#define GPIO_OUTPUT_W1TS_REG 0x60004008
#define GPIO_OUTPUT_W1TC_REG 0x6000400C

// Funções auxiliares
void delay(int ms) {
    for (volatile int i = 0; i < ms * 1000; i++);
}

// Função Principal
void app_main(void)
{
    REG_WRITE(GPIO_ENABLE_REG, 0x100); // Ativa o pino da posição 8 (lembre-se que começa com 0) como saída
    while (1) {
        REG_WRITE(GPIO_OUTPUT_W1TS_REG, 0x100); // Coloca o pino da posição 8 para nível lógico ALTO
        delay(10000);
        REG_WRITE(GPIO_OUTPUT_W1TC_REG, 0x100);  // Coloca o pino da posição 8 para nível lógico BAIXO
        delay(10000);
    }
}