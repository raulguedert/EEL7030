#define GPIO_ENABLE_REG 0x60004020
#define GPIO_OUT_W1TS_REG 0x60004008
#define GPIO_OUT_W1TC_REG 0x6000400C

struct RegsGPIO
{
    unsigned long int *ENABLE;
    unsigned long int *OUT_W1TS;
    unsigned long int *OUT_W1TC;
};

void delay(int ms){
    for(volatile int i = 0; i< ms * 1000; i++);
};

void app_main(void ){
    struct RegsGPIO GPIO;
    GPIO.ENABLE = (unsigned long int *)GPIO_ENABLE_REG;
    GPIO.OUT_W1TS = (unsigned long int *)GPIO_OUT_W1TS_REG;
    GPIO.OUT_W1TC = (unsigned long int *)GPIO_OUT_W1TC_REG;

    *GPIO.ENABLE = 0xF;
    *GPIO.OUT_W1TS = 0xF;

    int n = 0;

    while(1){
        *GPIO.OUT_W1TC = n;
        delay(10000);
        *GPIO.OUT_W1TS = n;
        
        if (n >= 15){
            n = 0;
        } else {
            n++;
        }
    }
};