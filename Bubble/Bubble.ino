/*
  COLOR_NONE,       //0x000000,        //彩灯关闭，colorLED OFF
  COLOR_RED,        //0xff0000,        //红色，Red
  COLOR_ORANGE,     //0xea8b17,        //橙色，Orange
  COLOR_YELLOW,     //0xffff00,        //黄色，Yellow
  COLOR_GREEN,      //0x00ff00,        //绿色，Green
  COLOR_LIME,       //0x00ffff,        //青色，Lime
  COLOR_BLUE,       //0x0000ff,        //蓝色，Blue
  COLOR_PURPLE,     //0xff00ff,        //紫色，Purple
  COLOR_WARM,       //0x9b6400,        //暖色，Warm
  COLOR_COLD,       //0x648264,        //冷色，Cold
*/

#include <Microduino_ColorLED.h>  //彩灯库
#include <Servo.h>               //舵机库

#define PIN_LED   10       //彩灯引脚
#define LED_NUM   1        //彩灯的数量
#define PIN_PIR   6       //人体红外引脚
#define PIN_SERVO 8       //舵机引脚

#define ANGLEDOWN 35
#define ANGLEUP   130

Servo servo;
ColorLED strip = ColorLED(LED_NUM, PIN_LED, NEO_GRB + NEO_KHZ800);

void servo_move(int target_angle, int move_speed) {
  int servo_angle;
  servo_angle = servo.read();
  if (move_speed >= 1000) {
    servo.write(target_angle);
  }
  else {
    if (target_angle - servo_angle > 0) {
      for (int a = 0; a < target_angle - servo_angle; a++) {
        servo.write(servo_angle + a);
        delay(1000 / move_speed);
      }
    }
    else if (target_angle - servo_angle < 0) {
      for (int a = 0; a < servo_angle - target_angle; a++) {
        servo.write(servo_angle - a);
        delay(1000 / move_speed);
      }
    } else {
      servo.write(target_angle);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_PIR, INPUT);
  strip.begin();
  strip.setBrightness(50);
  servo.attach(PIN_SERVO);
  servo_move(ANGLEDOWN, 50);
}

void loop() {

  //Serial.println(digitalRead(PIN_PIR));

  if (digitalRead(PIN_PIR))                   //有人体感应
  {
    strip.setPixelColor(0, COLOR_WARM);     //设置相应灯的颜色
    strip.show();                           //将灯点亮成新的颜色
    for (uint8_t i = 0; i < 2; i++)         //泡泡机吹两次泡泡
    {
      servo_move(ANGLEUP, 50);             //舵机运行到最上，吹泡泡
      delay(2000);                         //吹泡泡时间，时间越长，泡泡越大
      servo_move(ANGLEDOWN, 50);          //舵机运行到最下，取泡泡液
      delay(300);
    }
  }
  else
  {
    servo_move(ANGLEDOWN, 50);              //舵机运行到最下
    strip.setPixelColor(0, COLOR_NONE);     //设置相应灯的颜色
    strip.show();                           //将灯点亮成新的颜色
  }
}
