/**
 * Ref http://idken.net/posts/2017-02-01-freertos_task_cpp/
 */

#ifndef TASK_TASKBASE_H_
#define TASK_TASKBASE_H_

#include <Arduino.h>
#include "FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

class TaskBase
{
public:
    // デフォルトコンストラクタを禁止
    // 継承先のコンストラクタでTaskBaseの初期化を強制するため
    TaskBase() = delete;

    TaskBase(const char *_name, int _priority, uint32_t _stack_size = configMINIMAL_STACK_SIZE)
        : name(_name), priority(_priority), stack_size(_stack_size)
    {
    }

    virtual ~TaskBase()
    {
        delete_task();
    }

    void create_task()
    {
        log_d("Starting %s.", name);
        xTaskCreate(task_entry_point, name, stack_size, this, priority, &handle);
    }

    void delete_task()
    {
        vTaskDelete(handle);
    }

protected:
    xTaskHandle handle = 0;
    const char *name;
    int priority;
    uint32_t stack_size;

    virtual void task() = 0;

    static void task_entry_point(void *task_instance)
    {
        static_cast<TaskBase *>(task_instance)->task();
    }
};

#endif /* TASK_TASKBASE_H_ */