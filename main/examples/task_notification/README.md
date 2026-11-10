# Task Notification: اعلان سبک بین دو Task

## هدف آموزشی

یاد بگیری چطور یک Task می‌تواند مستقیماً Task دیگر را **بیدار** کند، با سبک‌ترین و سریع‌ترین روش FreeRTOS.

## ایده‌ی ساده

در مثال قبلی (Semaphore) یک «زنگ» جدا داشتیم. اینجا **زنگ به خود Task وصل است**. سنسور مستقیماً به پردازنده زنگ می‌زند و لازم نیست واسطه‌ای وجود داشته باشد.

## مقایسه‌ی سریع

| | Task Notification | Binary Semaphore |
| --- | --- | --- |
| حافظه و سرعت | خیلی سبک و سریع | کمی سنگین‌تر |
| تعداد گیرنده | فقط یک Task مشخص | هر Taskی که منتظر آن باشد |
| شمارش | می‌تواند تعداد اعلان‌ها را بشمارد | فقط خالی یا پر |

## مفاهیم کلیدی

| تابع | کاربرد |
| --- | --- |
| `xTaskCreate(..., &handle)` | گرفتن handle (شناسه) Task برای ارسال اعلان به آن |
| `xTaskNotifyGive(handle)` | ارسال اعلان به Task مشخص |
| `ulTaskNotifyTake(pdTRUE, ...)` | منتظر ماندن برای اعلان و برگرداندن تعداد آن‌ها |

## نحوه‌ی کار

- **`sensor_task`** هر ثانیه یک بار `xTaskNotifyGive(worker_handle)` را صدا می‌زند.
- **`worker_task`** با `ulTaskNotifyTake` منتظر می‌ماند. مقدار برگشتی تعداد اعلان‌های دریافت‌نشده است.
- پارامتر `pdTRUE` یعنی بعد از دریافت، شمارنده به صفر برگردد.

## خروجی انتظار

```text
Sensor event
Worker notified: 1
Processing...
Sensor event
Worker notified: 1
Processing...
...
```

## اجرا

در [main/main.c](../../main.c):

```c
#include "task_notification_example.h"

void app_main(void)
{
    task_notification_example_run();
}
```

## تمرین

1. تأخیر در `sensor_task` را کم کن و یک `vTaskDelay` بزرگ داخل `worker_task` بگذار. وقتی Worker مشغول است، اعلان‌ها جمع می‌شوند و در خروجی عدد بزرگ‌تر از ۱ می‌بینی.
2. با `ulTaskNotifyTake(pdFALSE, ...)` امتحان کن. این بار شمارنده کاهش نمی‌یابد و باید آن را خودت مدیریت کنی.
3. یک Task سوم بساز و ببین چرا برای ارسال اعلان به چند Task باید از Semaphore یا Queue استفاده کرد.
