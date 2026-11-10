# Binary Semaphore: اعلام آماده بودن داده

## هدف آموزشی

یاد بگیری چطور یک Task می‌تواند به Task دیگر **علامت بدهد** که کاری آماده است، بدون اینکه Task دوم مدام منتظر یا در حال چک کردن باشد.

## ایده‌ی ساده

فرض کن یک **زنگ** بین سنسور و پردازنده داری. وقتی سنسور داده‌ای آماده کند، زنگ را می‌زند. پردازنده تا زمانی که زنگ نخورد استراحت می‌کند.

به این زنگ در FreeRTOS **Binary Semaphore** می‌گویند. دو حالت دارد: **خالی** (صفر) یا **پر** (یک).

## مفاهیم کلیدی

| تابع | کاربرد |
| --- | --- |
| `xSemaphoreCreateBinary()` | ساخت semaphore (در ابتدا خالی است) |
| `xSemaphoreGive()` | علامت دادن (پر کردن) |
| `xSemaphoreTake()` | منتظر ماندن و مصرف علامت (خالی کردن) |

## نحوه‌ی کار

- **`sensor_task`** هر ثانیه یک بار "داده آماده است" را چاپ می‌کند و `xSemaphoreGive` را صدا می‌زند.
- **`processor_task`** با `xSemaphoreTake(..., portMAX_DELAY)` منتظر می‌ماند. تا علامت نرسد، Task بیدار نمی‌شود و CPU مصرف نمی‌کند.

## خروجی انتظار

```text
Sensor: data ready
Processor: reading sensor
Sensor: data ready
Processor: reading sensor
...
```

## اجرا

در [main/main.c](../../main.c):

```c
#include "semaphore_example.h"

void app_main(void)
{
    semaphore_example_run();
}
```

## تمرین

1. تأخیر سنسور را از `1000` به `200` میلی‌ثانیه کم کن. آیا همه‌ی علامت‌ها پردازش می‌شوند؟
2. اگر سنسور چند بار پشت سر هم `xSemaphoreGive` بزند ولی پردازنده فوراً نگیرد، چه می‌شود؟ (نکته: Binary Semaphore فقط یک علامت را نگه می‌دارد.)
3. برای دیدن فرق، یک **Counting Semaphore** با `xSemaphoreCreateCounting()` امتحان کن.

## Semaphore در مقابل Mutex

| | Binary Semaphore | Mutex |
| --- | --- | --- |
| هدف | اعلام رویداد (علامت) | محافظت از داده |
| چه کسی آزاد می‌کند | می‌تواند Task دیگری باشد | فقط همان Taskی که گرفته |
| کاربرد نمونه | "داده آماده است" | "این متغیر را فقط من تغییر می‌دهم" |
