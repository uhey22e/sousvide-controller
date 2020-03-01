#include <Arduino.h>
#include <freertos/queue.h>
#include "display_task.h"

void DisplayTask::task()
{
    u8g2.begin();

    const char *helloMessage = "HELLO";
    u8g2.firstPage();
    do
    {
        u8g2.setFont(font2);
        u8g2.drawStr(alignCenter(helloMessage), 32, helloMessage);
    } while (u8g2.nextPage());

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
                u8g2.setFont(font2);
                u8g2.drawUTF8(alignRight(buf), 32, buf);
                if (indicator)
                {
                    u8g2.drawDisc(10, 10, 5);
                }
            } while (u8g2.nextPage());
            vPortFree((void *)buf);
        }
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