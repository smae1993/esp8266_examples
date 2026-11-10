#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

static SemaphoreHandle_t counter_mutex;
static int total = 0;

static void worker_task(void *arg)
{
    const char *name = (const char *)arg;

    while (1) {

        // قبل از تغییر مقدار، قفل بگیر
        if (xSemaphoreTake(
                counter_mutex,
                portMAX_DELAY
            ) == pdTRUE) {

            total++;

            int current = total;

            // کار با داده مشترک تمام شد
            xSemaphoreGive(counter_mutex);

            printf(
                "%s: total = %d\n",
                name,
                current
            );
        }

        // تاخیر خارج از Mutex
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void mutex_example_run(void)
{
    counter_mutex = xSemaphoreCreateMutex();

    if (counter_mutex == NULL) {
        printf("Mutex creation failed!\n");
        return;
    }

    xTaskCreate(
        worker_task, "worker_A",
        3072, "Task A", 5, NULL
    );

    xTaskCreate(
        worker_task, "worker_B",
        3072, "Task B", 5, NULL
    );
}