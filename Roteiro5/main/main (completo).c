#define GPIO_ENABLE_REG               0x60004020
#define GPIO_OUT_REG                  0x60004004
#define GPIO_OUTPUT_W1TS_REG          0x60004008
#define GPIO_OUTPUT_W1TC_REG          0x6000400C
#define IO_MUX_GPIOn_REG(n)           (0x60009004+4*n)
#define GPIO_FUNCn_OUT_SEL_CFG_REG(n) (0x60004554+4*n)

void REG_WRITE(unsigned int address, unsigned int value);
void IO_MUX_set_as_GPIO(int ports[], int length);
void delay(int ms);
void LCD_escreve_dado(unsigned char dado);
void LCD_escreve_comando(unsigned char comando);
void LCD_escreve_caractere(unsigned char caractere);
void LCD_escreve_string(unsigned char str[]);
void LCD_inicializa(void );
void LCD_escreve_strings(unsigned char str1[], unsigned char str2[]);

void app_main(){
   int ports[] = {0,1,2,3,4,5,6,7,8,9};
   int length = sizeof(ports) / sizeof(ports[0]);
   unsigned char nome[] = "EEL7030 - MIC";
   unsigned char prof1[] = "Prof. Bezerra";
   unsigned char prof2[] = "Prof. Lucas";
   unsigned char prof3[] = "Prof. Raul";

   IO_MUX_set_as_GPIO(ports, length);
   REG_WRITE(GPIO_ENABLE_REG, 0x7FF);
   REG_WRITE(GPIO_OUT_REG, 0x0);

   LCD_inicializa();

   while(1){
      LCD_escreve_strings(nome, prof1);
      delay(50000);
      LCD_escreve_strings(nome, prof2);
      delay(50000);
      LCD_escreve_strings(nome, prof3);
      delay(50000);
   } 
}
 
void IO_MUX_set_as_GPIO(int ports[], int length){
   int n;
   for(int i = 0; i < length; i++){
      n = ports[i];
      REG_WRITE(IO_MUX_GPIOn_REG(n), 0x1800);
      REG_WRITE(GPIO_FUNCn_OUT_SEL_CFG_REG(n), 0x280);
   } 
}

void REG_WRITE(unsigned int address, unsigned int value){
   unsigned int *ptr_address = (unsigned int *)address;
   *ptr_address = value;
}

void LCD_inicializa(void ){
   LCD_escreve_comando(0x0C);
   LCD_escreve_comando(0x3C); 
   LCD_escreve_comando(0x01); 
   LCD_escreve_comando(0x02); 
}

void delay(int ms) {
    for (volatile int i = 0; i < ms * 1000; i++);
}
 
void LCD_escreve_strings(unsigned char str1[], unsigned char str2[]){
   LCD_escreve_comando(0x80);
   LCD_escreve_string(str1);
   LCD_escreve_comando(0xC0);
   LCD_escreve_string(str2);
}

void LCD_escreve_comando(unsigned char comando){
    REG_WRITE(GPIO_OUTPUT_W1TC_REG, (1 << 8));
    LCD_escreve_dado(comando);
}

void LCD_escreve_string(unsigned char str[]){
   int i = 0;
   while ((str[i] != 0x0) && (i < 16)) {
      LCD_escreve_caractere(str[i]);
      i++;
   }
   while (i < 16) {
      LCD_escreve_caractere(' ');
      i++; 
   }
}

void LCD_escreve_caractere(unsigned char caractere){
    REG_WRITE(GPIO_OUTPUT_W1TS_REG, (1 << 8));
    LCD_escreve_dado(caractere);
}

void LCD_escreve_dado(unsigned char dado) {
    REG_WRITE(GPIO_OUTPUT_W1TC_REG, 0xFF);
    REG_WRITE(GPIO_OUTPUT_W1TS_REG, dado);
    REG_WRITE(GPIO_OUTPUT_W1TS_REG, (1 << 9));
    delay(10);
    REG_WRITE(GPIO_OUTPUT_W1TC_REG, (1 << 9));
}