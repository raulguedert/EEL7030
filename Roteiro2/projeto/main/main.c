// Código base fornecido no início da aula

#include "soc/soc.h"

void delay(int ms) {
    for (volatile int i = 0; i < ms * 1000; i++);
}

void app_main(void)
{
    REG_WRITE(0x60004020, BIT8);
    while (1) {
        REG_WRITE(0x60004008, BIT8);
        delay(10000);
        REG_WRITE(0x6000400C, BIT8);
        delay(10000);
    }
}