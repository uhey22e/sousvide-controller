#include <Arduino.h>
#include <U8g2lib.h>
#include <cstdio>
#include <Wire.h>
#include <OneWire.h>
#include "display_task.h"
#include "pin_config.h"
#include "ds18b20.h"

// Enable U8G2_16BIT in u8g2.h
U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

DS18B20 ds18b20(PinConfig::ds18b20_data);
DisplayTask displayTask;

void setup()
{
  Serial.begin(115200);
  ds18b20.init();
  displayTask.create_task();
}

void loop()
{
  delay(1000);
  // unsigned long time = millis();
  // unsigned long min = (time / 1000) / 60;
  // unsigned long sec = (time / 1000) % 60;
  // char buf[16];
  // sprintf(buf, "%02ld:%02ld", min, sec);
  // log_d("Sending message: %s", buf);
  // displayTask.displayMessage(buf);

  uint32_t convTime = ds18b20.requestTemperature();
  delay(convTime);
  float temperature = ds18b20.getTemperatureC();
  log_i("Temp = %.4f deg-C", temperature, temperature);

  char buf[16];
  sprintf(buf, "%2.2f %lcC", temperature, 0xb0);
  displayTask.displayMessage(buf);
}