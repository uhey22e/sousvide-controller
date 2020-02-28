#include <Arduino.h>
#include "ds18b20.h"
#include "display_task.h"
#include "pin_config.h"

static DisplayTask displayTask;
static DS18B20 ds18b20(PinConfig::ds18b20_data);

void setup()
{
  ds18b20.init();
  displayTask.create_task();
}

void loop()
{
  delay(1000);

  uint32_t convTime = ds18b20.requestTemperature();
  delay(convTime);
  float temperature = ds18b20.getTemperatureC();
  log_i("Temp = %.4f deg-C", temperature, temperature);

  char buf[16];
  sprintf(buf, "%2.2f %lcC", temperature, 0xb0);

  displayTask.displayMessage(buf);
}
