#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// نوع فرمانی که می‌خواهیم منتقل کنیم
typedef struct {
    int relay_id;
    int turn_on;
} RelayCommand;

// Handle صف فرمان‌ها
static QueueHandle_t command_queue;

// Task ارسال‌کننده فرمان
static void network_task(void *arg)
{
    RelayCommand command;

    while (1) {

        // درخواست روشن شدن رله شماره 1
        command.relay_id = 1;
        command.turn_on = 1;

        // ارسال یک کپی از command
        xQueueSend(
            command_queue,
            &command,
            portMAX_DELAY
        );

        vTaskDelay(pdMS_TO_TICKS(2000));

        // درخواست خاموش شدن
        command.turn_on = 0;

        xQueueSend(
            command_queue,
            &command,
            portMAX_DELAY
        );

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

// Task دریافت‌کننده فرمان
static void relay_task(void *arg)
{
    RelayCommand command;

    while (1) {

        // تا دریافت پیام منتظر بمان
        if (xQueueReceive(
                command_queue,
                &command,
                portMAX_DELAY
            ) == pdTRUE) {

            printf(
                "Relay %d: %s\n",
                command.relay_id,
                command.turn_on ? "ON" : "OFF"
            );
        }
    }
}

void queue_example_run(void)
{
    // ظرفیت 5 فرمان از نوع RelayCommand
    command_queue = xQueueCreate(
        5,
        sizeof(RelayCommand)
    );

    if (command_queue == NULL) {
        printf("Queue creation failed!\n");
        return;
    }

    // ایجاد Taskها
    BaseType_t a = xTaskCreate(
        network_task,
        "network",
        3072,
        NULL,
        5,
        NULL
    );

    BaseType_t b = xTaskCreate(
        relay_task,
        "relay",
        3072,
        NULL,
        5,
        NULL
    );

    if (a != pdPASS || b != pdPASS) {
        printf("Task creation failed!\n");
    }
}