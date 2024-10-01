#define GPIO_ENABLE_REG 0x60004020
#define GPIO_OUT_W1TS_REG 0x60004008
#define GPIO_OUT_W1TC_REG 0x6000400C
#define IO_MUX_GPIOn_REG(n) (0x60009004 + 4*n)
#define GPIO_FUNCn_OUT_SEL_CFG_REG(n) (0x60004554 + 4*n)

void REG_WRITE(unsigned int address, unsigned int value);
void IO_MUX_set_as_GPIO(int ports[], int length);
void delay(int ms);

void app_main(void ){
    int portas_io[] = {0,1,2,3,4,5,6,7};
    int tamanho = sizeof(portas_io) / sizeof(portas_io[0]);
    IO_MUX_set_as_GPIO(portas_io, tamanho);

    REG_WRITE(GPIO_ENABLE_REG, 0xF);
    REG_WRITE(GPIO_OUT_W1TS_REG, 0xF);

    int n = 0;

    while(1){
        REG_WRITE(GPIO_OUT_W1TC_REG, n);
        delay(10000);
        REG_WRITE(GPIO_OUT_W1TS_REG, n);
        
        if (n >= 15){
            n = 0;
        } else {
            n++;
        }
    }
};

void REG_WRITE(unsigned int address, unsigned int value){
    unsigned int *ptr_address = (unsigned int *)address;
    *ptr_address = value;
};

void IO_MUX_set_as_GPIO(int ports[], int length){
    int n;
    for(int i = 0; i < length; i++){
        n = ports[i];
        REG_WRITE(IO_MUX_GPIOn_REG(n), 0x1800);
        REG_WRITE(GPIO_FUNCn_OUT_SEL_CFG_REG(n), 0x280);
    }
};

void delay(int ms){
    for(volatile int i = 0; i< ms * 1000; i++);
};