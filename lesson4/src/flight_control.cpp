//Lesson 2
//スロットルレーバーでモータの回転が変わるようにする
#include "flight_control.hpp"
#include "motor.hpp"
#include "rc.hpp"
#include "led.hpp"

//Global variable
const float Control_period = 0.0025f;//400Hz //制御周期
volatile uint8_t Loop_flag = 0;
uint32_t Loop_counter = 0;

//割り込み関数
//Intrupt function
hw_timer_t * timer = NULL;
void IRAM_ATTR onTimer() 
{
  Loop_flag = 1;
}

//割り込み設定
void init_interrupt(void)
{
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 2500, true);
  timerAlarmEnable(timer);
}

//Initialize Multi copter
void init_copter(void)
{
  //Initialize Serial communication
  USBSerial.begin(115200);
  delay(1000);
  USBSerial.printf("Init StampFly!\r\n");

  //モータ設定
  init_motor();
  //RC設定
  init_rc();
  //LED設定
  init_led(0);

  //割り込み設定
  init_interrupt();
  USBSerial.printf("Join StampFly!\r\n");
}

//Main loop
void loop_400Hz(void)
{
  //以下は変更しない
  while(Loop_flag==0);
  Loop_flag = 0;
  //以上は変更しない

  //Start of Loop_400Hz function
  //以下に記述したコードが400Hzで繰り返される
  
  //LED10秒動後に変化する
  if (Loop_counter < 4000)
  {
    board_bottom_led(RED, 1);
    board_tail_led(GREEN, 1);
    stamp_led(BLUE, 1);  
  }
  else
  {
    board_bottom_led(YELLOW, 1);
    board_tail_led(YELLOW, 1);
    stamp_led(YELLOW, 1);      
  }

  

  float FR_duty = (6.2e+2*Stick[THROTTLE])/1000  - (6.2e+2*Stick[AILERON])/1000  + (6.2e+2*Stick[ELEVATOR])/1000 + (3.94e+7*Stick[RUDDER])/25000000;
  float RR_duty = (6.2e+2*Stick[THROTTLE])/1000  - (6.2e+2*Stick[AILERON])/1000  - (6.2e+2*Stick[ELEVATOR])/1000 - (3.94e+7*Stick[RUDDER])/25000000;
  float RL_duty = (6.2e+2*Stick[THROTTLE])/1000  + (6.2e+2*Stick[AILERON])/1000  - (6.2e+2*Stick[ELEVATOR])/1000 + (3.94e+7*Stick[RUDDER])/25000000;
  float FL_duty = (6.2e+2*Stick[THROTTLE])/1000  + (6.2e+2*Stick[AILERON])/1000  + (6.2e+2*Stick[ELEVATOR])/1000 - (3.94e+7*Stick[RUDDER])/25000000;



  if(FR_duty >= 0.9)FR_duty = 0.9;
  if(FL_duty >= 0.9)FL_duty = 0.9;
  if(RR_duty >= 0.9)RR_duty = 0.9;
  if(RL_duty >= 0.9)RL_duty = 0.9;

  if(FR_duty < 0.1)FR_duty = 0;
  if(FL_duty < 0.1)FL_duty = 0;
  if(RR_duty < 0.1)RR_duty = 0;
  if(RL_duty < 0.1)RL_duty = 0;

  

  set_motor_duty(FRONT_RIGHT_MOTOR ,FR_duty);
  set_motor_duty(FRONT_LEFT_MOTOR ,FL_duty);
  set_motor_duty(REAR_LEFT_MOTOR ,RL_duty);
  set_motor_duty(REAR_RIGHT_MOTOR ,RR_duty);

  USBSerial.printf(":%f:%f:%f:%f\n",FR_duty, FL_duty, RL_duty, RR_duty);  
  
  
  



  Loop_counter ++ ;

  //1秒ごとにLoop_counterの値を端末に表示
  if(Loop_counter%400==0) 
    USBSerial.printf("%6d\n\r", Loop_counter);
  
  FastLED.show(64);
  //End of Loop_400Hz function
}
