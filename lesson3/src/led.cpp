#include "led.hpp"

CRGB Stamp_led[1];
CRGB Board_led[2];

uint16_t LED_counter=0;
int ledCase = 0;
int caseCount = 0;

void init_led(uint8_t state)
{
  //Initialaze LED function

  //LED 初期化
  FastLED.addLeds<WS2812, BOARD_LED_PIN, GRB>(Board_led, 2);
  FastLED.addLeds<WS2812, STAMP_LED_PIN, GRB>(Stamp_led, 1);

  //色設定
  Board_led[0] = RED;
  Stamp_led[TAIL_LED] = WHITE;
  Stamp_led[BOTTOM_LED] = BLUE;
  
  //発色
  if (state!=0) FastLED.show();
}

void board_tail_led(CRGB color, uint8_t state)
{
    if(state!=0) Board_led[TAIL_LED] = color;
    else Board_led[TAIL_LED] = 0x000000;
}

void board_bottom_led(CRGB color, uint8_t state)
{
    if(state!=0) Board_led[BOTTOM_LED] = color;
    else Board_led[BOTTOM_LED] = 0x000000;
}

void stamp_led(CRGB color, uint8_t state)
{
    if(state!=0) Stamp_led[0] = color;
    else Stamp_led[0] = 0x000000;
}


void blink_led(void)
{
    if(caseCount == 100){
        ledCase = rand() % 5 + 1;
        caseCount = 0;
    } 
    else{
        caseCount++;
    }
    
    USBSerial.printf("%d\n", ledCase);
    //ここに自分のコードを追加する（引数が必要なら変更指定良い）
    switch (ledCase)
    {
    case 1:
        board_tail_led(WHITE,1);
        board_bottom_led(RED,1);
        stamp_led(GREEN,1);
        break;
    
    case 2:
        board_tail_led(BLUE,1);
        board_bottom_led(PERPLE,1);
        stamp_led(YELLOW,1);
        break;
    
    case 3:
        board_tail_led(RED,1);
        board_bottom_led(GREEN,1);
        stamp_led(BLUE,1);
        break;
    
    case 4:
        board_tail_led(PERPLE,1);
        board_bottom_led(YELLOW,1);
        stamp_led(RED,1);
        break;
    
    case 5:
        board_tail_led(GREEN,1);
        board_bottom_led(BLUE,1);
        stamp_led(PERPLE,1);
        break;
    
    default:
        break;
    }
}
