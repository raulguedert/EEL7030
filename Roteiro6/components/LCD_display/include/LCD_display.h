void LCD_escreve_comando(char comando);
void LCD_escreve_caractere(char caractere);
void LCD_escreve_string(char str[]);
void LCD_inicializa_8_bits(char rs, char en, char d0, char d1, char d2, char d3, char d4, char d5, char d6, char d7);
void LCD_inicializa_4_bits(char rs, char en, char d4, char d5, char d6, char d7);
void LCD_escreve_strings(char str1[], char str2[]);