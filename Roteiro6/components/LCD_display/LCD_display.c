#include "LCD_display.h"
#include "driver/gpio.h"
#include "soc/gpio_reg.h"

static int codificar_portas_dados(char dado);
static void delay(int ms);
static void inicializa_portas_saida_GPIO(int mascara_binaria);
static void LCD_escreve(void );
static void LCD_escreve_dado(char dado);


typedef struct
{
    char modo_4_bits, D0, D1, D2, D3, D4, D5, D6, D7, RS, EN;
} LCD_struct;

static LCD_struct LCD;

static void delay(int ms){
  for(volatile int i = 0; (i < ms*1000); i++);
}

static void inicializa_portas_saida_GPIO(int mascara_binaria){
    // Reinicialização de cada pino
    for (int i = 0; i < 32; i++) { // Considerando que int tem 32 bits
        if (mascara_binaria & (1 << i)) { // verifica se o bit i está configurado como 1
            gpio_reset_pin(i);
            // Esta função reinicializa as configurações do pino e também configura o IOMUX para este pino na função GPIO,
            // desconectando qualquer outra saída de periférico (similar a função que desenvolvemos no Roteiro 4).
        }
    }

    // Configuração como pinos de saída
    gpio_config_t io_conf;
    io_conf.pin_bit_mask = mascara_binaria; // Máscara para os pinos
    io_conf.mode = GPIO_MODE_OUTPUT; // Configura como saída
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // Desativa pull-down
    io_conf.pull_up_en =  GPIO_PULLUP_DISABLE; // Desativa pull-up
    io_conf.intr_type = GPIO_INTR_DISABLE; // Interrupcao desativada

    gpio_config(&io_conf); // Atribui as configuracoes
};

static int codificar_portas_dados(char dado){
    // Essa função será responsável por receber o dado e codificá-lo 
    // para as portas correspondentes fornecidas na inicialização do componente
    // Essa etapa é necessária pois qualquer conjunto de portas pode ser utilizado
    // para formação do barramento de dados (não apenas 0, 1, 2, 3, 4, 5, 6, 7)
    //
    // O funcionamento utiliza um deslocamento da posição respectiva do bit para a primeira posição
    // seguido do uso de uma máscara para remoção dos demais bits. Em seguida, esse bit é deslocado
    // para a posição correspondente de cada porta. Ao final, todos os valores são somados, resultando
    // no valor a ser passado pelos registradores W1TS e W1TC para escrita no barramento de dados
    int codificado = 0;

    if (LCD.modo_4_bits == 1) { // No modo 4 bits, apenas as portas D4, D5, D6 e D7 são utilizadas
        codificado = (((dado >> 0) & 1) << LCD.D4)
                | (((dado >> 1) & 1) << LCD.D5)
                | (((dado >> 2) & 1) << LCD.D6)
                | (((dado >> 3) & 1) << LCD.D7);
    } else {
        codificado = (((dado >> 0) & 1) << LCD.D0)
                | (((dado >> 1) & 1) << LCD.D1)
                | (((dado >> 2) & 1) << LCD.D2)
                | (((dado >> 3) & 1) << LCD.D3)
                | (((dado >> 4) & 1) << LCD.D4)
                | (((dado >> 5) & 1) << LCD.D5)
                | (((dado >> 6) & 1) << LCD.D6)
                | (((dado >> 7) & 1) << LCD.D7);
    }

    return codificado;
};

static void LCD_escreve(void){
    // Função responsável por executar o processo de envio do dado para o LCD
    // Inicializa o pino do Enable em 1, aguarda o tempo necessário e altera para 0
    REG_WRITE(GPIO_OUT_W1TS_REG, (1 << LCD.EN));
    delay(50);
    REG_WRITE(GPIO_OUT_W1TC_REG, (1 << LCD.EN));
}

static void LCD_escreve_dado(char dado){
    // Função para escrita do dado no LCD. A função recebe o dado, codifica ele conforme as portas selecionadas na
    // inicialização e realiza o processo de escrita
    if (LCD.modo_4_bits == 1){
        // No caso do modo 4 bits, o dado é enviado em duas parcelas (primeiro os MSBs e depois os LSBs)

        // Envio dos 4 bits mais significativos
        REG_WRITE(GPIO_OUT_W1TC_REG, codificar_portas_dados(0xF)); // Limpa o barramento de dados (note que é necessário codificar nas portas corretas)
        REG_WRITE(GPIO_OUT_W1TS_REG, codificar_portas_dados((dado  >> 4) & 0xF)); // Escreve os 4 bits mais significativos no barramento
        LCD_escreve(); // Executa o procedimento para escrita

        // Envio dos 4 bits menos significativos
        REG_WRITE(GPIO_OUT_W1TC_REG, codificar_portas_dados(0xF)); // Limpa o barramento de dados
        REG_WRITE(GPIO_OUT_W1TS_REG, codificar_portas_dados(dado & 0xF)); // Escreve os bits menos significativos no barramento
        LCD_escreve(); // Executa o procedimento para escrita
    } else {
        // No caso do modo 8 bits, o dado é enviado de uma única vez
        REG_WRITE(GPIO_OUT_W1TC_REG, codificar_portas_dados(0xFF)); // Limpa o barramento de dados
        REG_WRITE(GPIO_OUT_W1TS_REG, codificar_portas_dados(dado)); // Escreve os bits no barramento
        LCD_escreve(); // Executa o procedimento para escrita
    }
};

void LCD_escreve_comando(char comando){
    // Função utilizada para escrever um comando, dessa forma o pino RS deve estar em 0
    REG_WRITE(GPIO_OUT_W1TC_REG, (1 << LCD.RS)); // Coloca o pino RS em 0
    LCD_escreve_dado(comando); // Escreve o comando
};

void LCD_inicializa_8_bits(char rs, char en, char d0, char d1, char d2, char d3, char d4, char d5, char d6, char d7){
    // Função utilizada para inicialização do LCD em modo 8 bits
    
    // Inicializa os pinos como saída
    inicializa_portas_saida_GPIO((1 << rs) | (1 << en) | (1 << d0) | (1 << d1) | (1 << d2) | (1 << d3) | (1 << d4) | (1 << d5) | (1 << d6) | (1 << d7));
    
    // Atribuição dos valores dos pinos fornecidos na inicialização para as variáveis globais estáticas do componente lcd_display
    LCD.RS = rs;
    LCD.EN = en;
    LCD.D0 = d0, LCD.D1 = d1, LCD.D2 = d2, LCD.D3 = d3, LCD.D4 = d4, LCD.D5 = d5, LCD.D6 = d6, LCD.D7 = d7;
    
    // Comandos de inicialização
    LCD_escreve_comando(0x0C); // Display ligado, cursor desligado
    LCD_escreve_comando(0x3C); // Ativa a segunda linha em modo 8 bits
    LCD_escreve_comando(0x01); // Limpa o display
    LCD_escreve_comando(0x02); // Retorna para o início
};

void LCD_inicializa_4_bits(char rs, char en, char d4, char d5, char d6, char d7){
    // Função utilizada para inicialização do LCD em modo 4 bits
    // Inicializa os pinos como saída
    inicializa_portas_saida_GPIO((1 << rs) | (1 << en) | (1 << d4) | (1 << d5) | (1 << d6) | (1 << d7));

    // Atribuição dos valores dos pinos fornecidos na inicialização para as variáveis globais estáticas do componente lcd_display
    LCD.RS = rs;
    LCD.EN = en;
    LCD.D4 = d4, LCD.D5 = d5, LCD.D6 = d6, LCD.D7 = d7;
    
    // Comandos de inicialização
    LCD_escreve_comando(0x20); // Altera para Modo 4 bits
    LCD.modo_4_bits = 1; // Altera a variável global estática para executar as funções em modo 4 bits
    LCD_escreve_comando(0x0C); // Display ligado, cursor desligado
    LCD_escreve_comando(0x28); // Ativa segunda linha em modo 4 bits
    LCD_escreve_comando(0x01); // Limpa o display
    LCD_escreve_comando(0x02); // Retorna para o início
};

void LCD_escreve_caractere(char caractere){
    // Função utilizada para escrever um caractere no display, dessa forma o pino RS deve estar em 1
    REG_WRITE(GPIO_OUT_W1TS_REG, (1 << LCD.RS)); // Atribui o valor 1 no pino RS
    LCD_escreve_dado(caractere); // Escreve o caractere
};

void LCD_escreve_string(char str[]){
    // Função utilizada para escrever uma sequência de caracteres (String)
    int i = 0;

    while ((str[i] != 0x0) && (i < 16)) { // Percorre toda a string e escreve cada um dos caracteres (limite de 16 caracteres)
        LCD_escreve_caractere(str[i]);
        i++;
    };

    while ((i < 16)) { // Se a string for menor que 16 caracteres, completa-os com espaços para limpar os respectivos caracteres no LCD
        LCD_escreve_caractere(' ');
        i++;
    };
};

void LCD_escreve_strings(char str1[], char str2[]){
    // Função utilizada para escrever duas strings, uma em cada linha do LCD
    LCD_escreve_comando(0x80); // Envia o comando para posicionar o cursor no início da primeira linha
    LCD_escreve_string(str1); // Escreve a primeira string
    LCD_escreve_comando(0xC0); // Envia o comando para posicionar o cursor no início da segunda linha
    LCD_escreve_string(str2); // Escreve a segunda string
};