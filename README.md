# مثال‌های آموزشی FreeRTOS روی ESP8266

این پروژه مجموعه‌ای از مثال‌های کوچک و جدا برای یادگیری **FreeRTOS** روی **ESP8266** است. هر مثال یک مفهوم مشخص را نشان می‌دهد و در پوشه‌ی خودش توضیح کامل دارد.

## پیش‌نیازها

- **ESP8266_RTOS_SDK** (در مسیر `../../ESP8266_RTOS_SDK` نسبت به پروژه)
- toolchain `xtensa-lx106-elf`
- یک برد ESP8266 (مثل NodeMCU یا Wemos D1) و کابل USB
- آشنایی اولیه با زبان C

## ساختار پروژه

```text
event_group_demo/   # پوشه‌ی پروژه (نام پروژه در بیلد: esp8266_examples)
├── main/
│   ├── main.c                  ← نقطه‌ی ورود؛ مثالی را که می‌خواهی اجرا کنی صدا می‌زند
│   ├── CMakeLists.txt
│   ├── component.mk
│   └── examples/
│       ├── event_group/        ← کد + README
│       ├── isr/
│       ├── mutex/
│       ├── queue/
│       ├── semaphore/
│       └── task_notification/
├── Makefile
└── CMakeLists.txt
```

## فهرست مثال‌ها

| مثال | مفهوم اصلی | توضیح کوتاه | راهنما |
| --- | --- | --- | --- |
| Mutex | محافظت از داده‌ی مشترک | دو Task یک شمارنده را امن افزایش می‌دهند | [examples/mutex](main/examples/mutex/README.md) |
| Semaphore | علامت دادن بین Taskها | سنسور به پردازنده خبر می‌دهد که داده آماده است | [examples/semaphore](main/examples/semaphore/README.md) |
| Queue | ارسال پیام بین Taskها | یک Task فرمان می‌فرستد و Task دیگر آن را می‌خواند | [examples/queue](main/examples/queue/README.md) |
| Task Notification | اعلان سبک و مستقیم | سنسور مستقیماً Worker را بیدار می‌کند | [examples/task_notification](main/examples/task_notification/README.md) |
| Event Group | منتظر ماندن برای چند شرط | Task تا روشن شدن Wi-Fi و سنسور صبر می‌کند | [examples/event_group](main/examples/event_group/README.md) |
| ISR | وقفه‌ی سخت‌افزاری | خواندن دکمه با Interrupt و بیدار کردن Task | [examples/isr](main/examples/isr/README.md) |

### ترتیب پیشنهادی برای یادگیری

1. **Mutex**: اول مشکل Race Condition را ببین.
2. **Semaphore**: فرق علامت دادن و قفل کردن را یاد بگیر.
3. **Queue**: انتقال داده بین Taskها.
4. **Task Notification**: روش سبک‌تر برای بیدار کردن یک Task.
5. **Event Group**: منتظر ماندن برای چند شرط همزمان.
6. **ISR**: کار با وقفه‌ی سخت‌افزاری.

## اجرای یک مثال

۱. فایل [main/main.c](main/main.c) را باز کن و مثالی را که می‌خواهی صدا بزن. مثلاً برای اجرای مثال Queue:

```c
#include "queue_example.h"

void app_main(void)
{
    queue_example_run();
}
```

۲. محیط SDK را فعال کن. در این پروژه با دستور `esp8266-enable` انجام می‌شود.

۳. بیلد کن:

```bash
make -j4
```

۴. برد را فلش کن و خروجی سریال را ببین:

```bash
make flash monitor ESPPORT=/dev/ttyUSB0
```

> همه‌ی مثال‌ها کامپایل می‌شوند، اما فقط مثالی که در `app_main` صدا زده شود اجرا می‌شود.

## اضافه کردن مثال جدید

1. یک پوشه‌ی جدید در `main/examples/` بساز، مثلاً `main/examples/my_example/`.
2. فایل‌های `my_example_example.c` و `my_example_example.h` را بساز. تابع اصلی باید `void my_example_example_run(void)` باشد و **`app_main` نداشته باشد**، چون `app_main` فقط در `main.c` است.
3. یک `README.md` کنار کد بنویس.
4. مسیر پوشه را در [main/component.mk](main/component.mk) زیر `COMPONENT_SRCDIRS` و `COMPONENT_ADD_INCLUDEDIRS` اضافه کن.
5. اگر با CMake بیلد می‌کنی، بعد از اضافه کردن فایل، CMake را دوباره اجرا کن (مثلاً `idf.py reconfigure`)، چون فایل‌ها با `GLOB` پیدا می‌شوند.
