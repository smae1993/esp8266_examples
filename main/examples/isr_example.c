#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_err.h"

#define BUTTON_GPIO GPIO_NUM_0

// Handle مربوط به Task دریافت‌کننده
static TaskHandle_t button_task_handle = NULL;

// این تابع در Context وقفه اجرا می‌شود
static void button_isr(void *arg)
{
    BaseType_t higher_priority_woken = pdFALSE;

    // فقط Task را مطلع کن
    vTaskNotifyGiveFromISR(
        button_task_handle,
        &higher_priority_woken
    );

    // در صورت نیاز، درخواست اجرای سریع‌تر Task
    if (higher_priority_woken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

// این تابع یک Task عادی است
static void button_task(void *arg)
{
    while (1) {

        // منتظر اعلان ISR بمان
        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        // حذف لرزش مکانیکی اولیه دکمه
        vTaskDelay(pdMS_TO_TICKS(30));

        // بررسی اینکه دکمه هنوز فشرده است
        if (gpio_get_level(BUTTON_GPIO) == 0) {

            printf("Button pressed!\n");

            // تا رها شدن دکمه صبر کن
            while (gpio_get_level(BUTTON_GPIO) == 0) {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
    }
}

void isr_example_run(void)
{
    // ابتدا Task را بساز
    BaseType_t result = xTaskCreate(
        button_task,
        "button_task",
        3072,
        NULL,
        10,
        &button_task_handle
    );

    if (result != pdPASS) {
        printf("Task creation failed!\n");
        return;
    }

    // تنظیم GPIO به عنوان ورودی با Pull-up
    gpio_config_t config = {
        .pin_bit_mask = 1ULL << BUTTON_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };

    ESP_ERROR_CHECK(gpio_config(&config));

    // نصب سرویس Interrupt برای GPIO
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    // ثبت ISR مخصوص دکمه
    ESP_ERROR_CHECK(
        gpio_isr_handler_add(
            BUTTON_GPIO,
            button_isr,
            NULL
        )
    );

    printf("Button ready!\n");
}