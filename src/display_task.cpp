#include <Arduino.h>
#include <freertos/queue.h>
#include "display_task.h"

void DisplayTask::task()
{
    u8g2.begin();
    // u8g2.setFont(u8g2_font_inb24_mr);
    u8g2.setFont(u8g2_font_inb30_mf);

    char *buf;

    while (1)
    {
        if (xQueueReceive(displayQueue, &buf, portMAX_DELAY) != pdPASS)
        {
            log_d("Failed to receive message.");
        }
        else
        {
            log_d("Reveive message: %s", buf);

            u8g2.firstPage();
            do
            {
                u8g2.drawStr(0, 31, buf);
            } while (u8g2.nextPage());
            vPortFree((void *)buf);
        }

        // vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void DisplayTask::displayMessage(const char *message)
{
    log_d("Sending message: %s", message);
    char *buf;
    buf = (char *)pvPortMalloc(strlen(message) + 1);

    strcpy(buf, message);
    if (xQueueSendToBack(displayQueue, (void *)&buf, portMAX_DELAY) != pdPASS)
    {
        log_d("Failed to send message.");
        vPortFree(buf);
    }
}