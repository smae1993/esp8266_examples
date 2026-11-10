#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static TaskHandle_t worker_handle = NULL;

// Task دریافت‌کننده
static void worker_task(void *arg)
{
    while (1) {

        // منتظر اعلان بمان
        uint32_t count = ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        printf(
            "Worker notified: %lu\n",
            (unsigned long)count
        );

        // انجام کار پس از اعلان
        printf("Processing...\n");
    }
}

// Task ارسال‌کننده
static void sensor_task(void *arg)
{
    while (1) {

        vTaskDelay(pdMS_TO_TICKS(1000));

        printf("Sensor event\n");

        // ارسال اعلان مستقیم
        xTaskNotifyGive(worker_handle);
    }
}

void task_notification_example_run(void)
{
    BaseType_t result = xTaskCreate(
        worker_task,
        "worker",
        3072,
        NULL,
        5,
        &worker_handle
    );

    if (result != pdPASS) {
        printf("Task creation failed!\n");
        return;
    }

    xTaskCreate(
        sensor_task,
        "sensor",
        3072,
        NULL,
        5,
        NULL
    );
}