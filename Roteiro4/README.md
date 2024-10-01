# Roteiro 4

## Problema a ser resolvido

Realizar a contagem de 0x0 a 0xF utilizando um display de 7 segmentos.

## Informações do Hardware

O display de 7 segmentos está ligado nos pinos GPIO do ESP32-C3 de forma que o MSB (most significant bit) está conectado ao segmento de ponto do display e o LSB (least significant bit) está conectado ao segmento a. Dessa forma, a codificação dos segmentos segue a ordem: "DP g f e d c b a". O display é do tipo ANODO COMUM, sendo sensível ao nível lógico BAIXO, onde os segmentos acendem quando o bit correspondente é definido para 0. Os pinos utilizados s"ao GPIO0 à GPIO7, de forma que o GPIO0 corresponde ao LSB.

## Exercícios

Abra o roteiro no Moodle da disciplina.