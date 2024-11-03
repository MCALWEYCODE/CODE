#include <Wire.h> 
#include "TLC59108.h" 
#include <Adafruit_MCP4725.h> 
#include <VL53L0X.h> 
 
#define HW_RESET_PIN 0 // Только программный сброс 
#define I2C_ADDR TLC59108::I2C_ADDR::BASE 
 
VL53L0X lox; 
Adafruit_MCP4725 buzzer; 
int vol1 = 700; 
int vol2 = 70; // разница значений = громкость 
int ton; 
TLC59108 leds(I2C_ADDR + 7); 
 
/* 
  Джамперы JP0, JP1, JP2 служат для выбора I2C-адреса. 
 
  Например: 
 
  Три надетых перемычки (000) - следует добавить 0 бит 000(bin) = 0(dec) = 
  TLC59108 leds(I2C_ADDR +0); 
 
  Ни одной перемычки (111) – следует добавить 3 бита 111(bin) = 7(dec) = 
  TLC59108 leds(I2C_ADDR +7); 
 
  Одна перемычка на JP0 (110) – следует добавить 2 старших бита бита 110(bin) = 6(dec) = 
  TLC59108 leds(I2C_ADDR +6); 
  (также попробуйте просканировать адрес: https://github.com/MAKblC/Codes/tree/master/I2C%20scanner) 
*/ 
 
// Задержка пустыми командами 
void delay_nop(int u) { 
  for (int i = 0; i < u; i++) { 
    _NOP(); 
  } 
} 
void note( int type, int duration) { // нота (нота, длительность) 
  switch (type) { 
    case 1: ton = 1000; break; 
    case 2: ton = 860; break; 
    case 3: ton = 800; break; 
    case 4: ton = 700; break; 
    case 5: ton = 600; break; 
    case 6: ton = 525; break; 
    case 7: ton = 450; break; 
    case 8: ton = 380; break; 
    case 9: ton = 315; break; 
    case 10: ton = 250; break; 
    case 11: ton = 190; break; 
    case 12: ton = 130; break; 
    case 13: ton = 80; break; 
    case 14: ton = 30; break; 
    case 15: ton = 1; break; 
  } 
  delay(10); // воспроизведение звука с определенной тональностью и длительностью 
  for (int i = 0; i < duration; i++) { 
    buzzer.setVoltage(vol1, false); 
    buzzer.setVoltage(vol2, false); 
    delayMicroseconds(ton); 
  } 
} 
void setup() { 
  // Инициализация последовательного порта 
  Serial.begin(115200); 
  // Инициализация модуля 
  Wire.begin(); 
  leds.init(HW_RESET_PIN); 
  buzzer.begin(0x61); // С перемычкой адрес будет 0x60 
  buzzer.setVoltage(0, false); // выключение звука 
  leds.init(HW_RESET_PIN); 
  buzzer.begin(0x60); 
  leds.setLedOutputMode(TLC59108::LED_MODE::PWM_IND); 
  lox.init(); 
  lox.setTimeout(500); 
  // параметры для режима высокой точности 
  lox.setMeasurementTimingBudget(200); 
} 
void loop() { 
    // Измерение 
  float dist = lox.readRangeSingleMillimeters(); 
  // Вывод измеренных значений в терминал 
  Serial.println("Distance  = " + String(dist, 0) + " mm  "); 
  delay(50); 
  if (dist > 100){ 
  /* leds.setBrightness(x, P); 
    x - светодиод: 
    0,6 - белые 
    1, 4 - УФ 
    3 - красный RGB 
    2 - зеленый RGB 
    5 - синий RGB 
    P - яркость (0x00-0xff) (0-255) 
  */ 
  for (byte pwm = 0; pwm < 0xff; pwm--) { 
    leds.setBrightness(3, pwm); 
    leds.setBrightness(3, pwm); 
    delay(0); 
  } 
  for (byte pwm = 0; pwm < 0xff; pwm++) { 
    leds.setBrightness(2, pwm); 
    leds.setBrightness(2, pwm); 
    delay(0); 
  } 
  } 
  else { 
  buzzer.setVoltage(0, false); // выключение звука 
  for (byte pwm = 0; pwm < 0xff; pwm--) { 
    leds.setBrightness(2, pwm); 
    leds.setBrightness(2, pwm); 
    delay(0); 
  } 
  for (byte pwm = 0; pwm < 0xff; pwm++) { 
    leds.setBrightness(3, pwm); 
    leds.setBrightness(3, pwm); 
    delay(0); 
  } 
  buzzer.setVoltage(0, false); // выключение звука 
  note(1, 450);
  } 
}