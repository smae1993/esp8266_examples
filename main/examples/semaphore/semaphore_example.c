#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

static SemaphoreHandle_t sensor_ready;

// Task شبیه‌ساز سنسور
static void sensor_task(void *arg)
{
    while (1) {

        vTaskDelay(pdMS_TO_TICKS(1000));

        printf("Sensor: data ready\n");

        // اعلام آماده بودن داده
        xSemaphoreGive(sensor_ready);
    }
}

// Task پردازشگر
static void processor_task(void *arg)
{
    while (1) {

        // منتظر سیگنال از سنسور بمان
        if (xSemaphoreTake(
                sensor_ready,
                portMAX_DELAY
            ) == pdTRUE) {

            printf("Processor: reading sensor\n");
        }
    }
}

void semaphore_example_run(void)
{
    // ساخت Semaphore با مقدار اولیه صفر
    sensor_ready = xSemaphoreCreateBinary();

    if (sensor_ready == NULL) {
        printf("Semaphore creation failed!\n");
        return;
    }

    xTaskCreate(
        processor_task, "processor",
        3072, NULL, 5, NULL
    );

    xTaskCreate(
        sensor_task, "sensor",
        3072, NULL, 5, NULL
    );
}