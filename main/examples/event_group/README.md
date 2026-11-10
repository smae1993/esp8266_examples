# Event Group: منتظر ماندن چند شرط

## هدف آموزشی

یاد بگیری چطور با **Event Group** چند «شرط» را کنار هم نگه داری و یک Task را تا وقتی همه‌ی شرط‌ها برقرار نشده‌اند منتظر کنی.

## ایده‌ی ساده

فرض کن برای شروع یک جلسه باید **Wi-Fi** و **سنسور** آماده باشند. هر کدام وقتی آماده شد، یک «چراغ» خودش را روشن می‌کند. مدیر جلسه صبر می‌کند تا هر دو چراغ روشن شوند و بعد جلسه را شروع می‌کند.

در FreeRTOS، این «چراغ‌ها» **بیت** هستند و مجموعه‌ی آن‌ها **Event Group** نام دارد.

## مفاهیم کلیدی

| تابع / ماکرو | کاربرد |
| --- | --- |
| `xEventGroupCreate()` | ساخت یک Event Group |
| `xEventGroupSetBits()` | روشن کردن یک یا چند بیت |
| `xEventGroupWaitBits()` | منتظر ماندن تا بیت‌های مشخص روشن شوند |
| `1U << n` | تعریف بیت شماره‌ی `n` (مثلاً `1U << 0` یعنی بیت صفر) |

## نحوه‌ی کار

1. `event_group_example_run()` یک Event Group می‌سازد و سه Task می‌سازد.
2. `wifi_task` بعد از ۱ ثانیه بیت `WIFI_READY` را روشن می‌کند.
3. `sensor_task` بعد از ۲ ثانیه بیت `SENSOR_READY` را روشن می‌کند.
4. `main_task` با این خط منتظر می‌ماند:

```c
xEventGroupWaitBits(system_events,
                    WIFI_READY | SENSOR_READY,  // کدام بیت‌ها
                    pdFALSE,                    // بعد از خروج پاک نشوند
                    pdTRUE,                     // منتظر همه باش، نه فقط یکی
                    portMAX_DELAY);             // تا ابد صبر کن
```

### پارامترهای `xEventGroupWaitBits`

- **`xClearOnExit`** (`pdFALSE`): اگر `pdTRUE` باشد، بیت‌ها بعد از برگشت پاک می‌شوند.
- **`xWaitForAllBits`** (`pdTRUE`): اگر `pdTRUE` باشد، Task فقط وقتی ادامه می‌دهد که **همه** بیت‌ها روشن باشند. اگر `pdFALSE` باشد، با روشن شدن **یکی** از آن‌ها ادامه می‌دهد.
- **`xTicksToWait`** (`portMAX_DELAY`): مدت انتظار. `portMAX_DELAY` یعنی بی‌نهایت.

## خروجی انتظار

```text
Waiting for system ready...
Wi-Fi connected
Sensor initialized
System is ready!
```

## اجرا

در [main/main.c](../../main.c) مثال را این‌طور صدا بزن:

```c
#include "event_group_example.h"

void app_main(void)
{
    event_group_example_run();
}
```

## تمرین

1. یک بیت سوم مثل `DISPLAY_READY` اضافه کن و Task اصلی را منتظر آن هم نگه دار.
2. `xWaitForAllBits` را `pdFALSE` کن. حالا با روشن شدن هر کدام از بیت‌ها ادامه می‌دهد؛ رفتار را مشاهده کن.
3. به جای `portMAX_DELAY` یک timeout کوتاه (مثلاً `pdMS_TO_TICKS(1500)`) بگذار. وقتی مهلت تمام شد، مقدار برگشتی `xEventGroupWaitBits` را بررسی کن.
