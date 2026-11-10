
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define WIFI_READY   (1U << 0)
#define SENSOR_READY (1U << 1)

static EventGroupHandle_t system_events;

static void wifi_task(void *arg)
{
    vTaskDelay(pdMS_TO_TICKS(1000));

    printf("Wi-Fi connected\n");

    xEventGroupSetBits(system_events, WIFI_READY);

    vTaskDelete(NULL);
}

static void sensor_task(void *arg)
{
    vTaskDelay(pdMS_TO_TICKS(2000));

    printf("Sensor initialized\n");

    xEventGroupSetBits(system_events, SENSOR_READY);

    vTaskDelete(NULL);
}

static void main_task(void *arg)
{
    printf("Waiting for system ready...\n");

    xEventGroupWaitBits(
        system_events,
        WIFI_READY | SENSOR_READY,
        pdFALSE,
        pdTRUE,
        portMAX_DELAY
    );

    printf("System is ready!\n");

    vTaskDelete(NULL);
}

void event_group_example_run(void)
{
    system_events = xEventGroupCreate();

    if (system_events == NULL) {
        printf("Event Group creation failed!\n");
        return;
    }

    xTaskCreate(wifi_task, "wifi", 3072, NULL, 5, NULL);
    xTaskCreate(sensor_task, "sensor", 3072, NULL, 5, NULL);
    xTaskCreate(main_task, "main_worker", 3072, NULL, 5, NULL);
}
