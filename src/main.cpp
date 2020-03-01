#include <Arduino.h>
#include <freertos/timers.h>
#include "ds18b20.h"
#include "display_task.h"
#include "acswitch.h"
#include "pin_config.h"

static DisplayTask displayTask;
static DS18B20 ds18b20(PinConfig::ds18b20_data);
static ACSwitch acSwitch(PinConfig::ssr);

TickType_t timerStartedAt;
const TickType_t timerDuration = pdMS_TO_TICKS(1 * 1000);
TimerHandle_t countdownTimer;

const float target_temp = 45;

void timerCallback(TimerHandle_t xTimer);

void setup()
{
  acSwitch.turnOff();

  ds18b20.init();
  displayTask.create_task();

  timerStartedAt = xTaskGetTickCount();
  countdownTimer = xTimerCreate("CountDown", timerDuration, pdTRUE, (void *)0, timerCallback);
  xTimerStart(countdownTimer, 0);
}

void loop()
{
  delay(100000);
}

void timerCallback(TimerHandle_t xTimer)
{
  uint32_t convTime = ds18b20.requestTemperature();
  delay(convTime);
  float temperature;
  if (ds18b20.getTemperatureC(&temperature) != OK)
  {
    log_e("Failed to measure temperature.");
    return;
  }
  log_i("Temp = %.4f deg-C", temperature, temperature);

  char buf[16];
  sprintf(buf, "%2.1f %lcC", temperature, 0xb0);

  displayTask.displayMessage(buf);

  if (temperature < (target_temp - 1))
  {
    acSwitch.turnOn();
    displayTask.setIndicator(true);
  }

  if (temperature > (target_temp + 1))
  {
    acSwitch.turnOff();
    displayTask.setIndicator(false);
  }
}