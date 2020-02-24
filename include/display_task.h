#ifndef __DISPLAY_TASK_H
#define __DISPLAY_TASK_H

#include <U8g2lib.h>
#include "taskbase.h"
#include <freertos/queue.h>

#define DISPLAY_TASK_NAME "DisplayTask"

class DisplayTask : public TaskBase
{
private:
    U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2;

    virtual void task();
    QueueHandle_t displayQueue;

public:
    DisplayTask() : TaskBase("DisplayTask", 1, 4096),
                    u8g2(U8G2_R0, U8X8_PIN_NONE)
    {
        displayQueue = xQueueCreate(4, sizeof(char *));
    }
    virtual ~DisplayTask()
    {
        vQueueDelete(displayQueue);
    }

    void displayMessage(const char *message);
};

#endif