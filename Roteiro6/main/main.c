#include "driver/gpio.h"
#include "LCD_display.h"

void delay(int ms){
  for(volatile int i = 0; (i < ms*1000); i++);
};

void app_main(){
};