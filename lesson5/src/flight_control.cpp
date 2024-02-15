//Lesson 2
//スロットルレーバーでモータの回転が変わるようにする
#include "flight_control.hpp"
#include "motor.hpp"
#include "rc.hpp"
#include "led.hpp"
#include "imu.hpp"

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
  //IMUの設定
  imu_init();
  imu_update();

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
  
  Loop_counter ++ ;


  USBSerial.printf("time:%d  acc_x:%f acc_y:%f acc_z:%f\n",Loop_counter / 400,imu_get_acc_x(), imu_get_acc_y(), imu_get_acc_z());
  USBSerial.printf("time:%d  gyro_x:%f gyro_y:%f gyro_z:%f\n",Loop_counter / 400,imu_get_gyro_x(), imu_get_gyro_y(), imu_get_gyro_z());


  if(imu_get_gyro_x() > 1) {
    board_bottom_led(RED, 1);
  }
  if(imu_get_gyro_y() > 1) {
    board_bottom_led(GREEN, 1);
  }
  if(imu_get_gyro_z() > 1) {
    board_bottom_led(BLUE, 1);
  }


  //1秒ごとにLoop_counterの値を端末に表示
  if(Loop_counter%400==0) 
    USBSerial.printf("%6d\n\r", Loop_counter);
  
  imu_update();
  FastLED.show();
  //End of Loop_400Hz function
}
