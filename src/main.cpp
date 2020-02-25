#include <Arduino.h>
#include <U8g2lib.h>
#include <cstdio>
#include <Wire.h>
#include <OneWire.h>
#include "display_task.h"

// Enable U8G2_16BIT in u8g2.h
U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

DisplayTask displayTask;
OneWire ds(27);

void setup()
{
  Serial.begin(115200);
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

  byte data[12];
  byte addr[8];

  ds.reset_search();
  if (!ds.search(addr))
  {
    log_e("No more addresses.");
    ds.reset_search();
    delay(250);
    return;
  }

  log_d("ROM = %02X %02X %02X %02X %02X %02X %02X",
        addr[0], addr[1], addr[2], addr[3],
        addr[4], addr[5], addr[6]);

  if (OneWire::crc8(addr, 7) != addr[7])
  {
    log_e("CRC is not valid.");
    return;
  }

  // the first ROM byte indicates which chip
  if (addr[0] != 0x28)
  {
    log_e("Chip is not a DS18B20.");
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end

  delay(1000); // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++)
  { // we need 9 bytes
    data[i] = ds.read();
  }
  log_d("Raw data = %02X %02X %02X %02X %02X %02X %02X %02X",
        data[0], data[1], data[2], data[3],
        data[4], data[5], data[6], data[7]);

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  byte cfg = (data[4] & 0x60);
  // at lower res, the low bits are undefined, so let's zero them
  if (cfg == 0x00)
    raw = raw & ~7; // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20)
    raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40)
    raw = raw & ~1; // 11 bit res, 375 ms
  //// default is 12 bit resolution, 750 ms conversion time
  float celsius = (float)raw / 16.0;
  log_i("Temp = %.4f deg-C", celsius, celsius);

  char buf[16];
  sprintf(buf, "%2.2f %lcC", celsius, 0xb0);
  displayTask.displayMessage(buf);
}