#define GPIO_ENABLE_REG 0x60004020
#define GPIO_OUT_W1TS_REG 0x60004008
#define GPIO_OUT_W1TC_REG 0x6000400C

#define pinMask (1 << 8)

void delay(int ms){
    for(volatile int i = 0; i< ms * 1000; i++);
}

void app_main(void ){
    unsigned long int *ptr_regOUT = (unsigned long int *)GPIO_ENABLE_REG;
    unsigned long int *ptr_regVAL1 = (unsigned long int *)GPIO_OUT_W1TS_REG;
    unsigned long int *ptr_regVAL0 = (unsigned long int *)GPIO_OUT_W1TC_REG;

    *ptr_regOUT = 0x100;

    while(1){
        *ptr_regVAL1 = pinMask;
        delay(10000);
        *ptr_regVAL0 = pinMask;
        delay(10000);
    }
}