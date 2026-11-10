#
# "main" pseudo-component makefile.
#
# Every example lives in its own folder under examples/<name>/

COMPONENT_SRCDIRS := . \
    examples/event_group \
    examples/isr \
    examples/mutex \
    examples/queue \
    examples/semaphore \
    examples/task_notification

COMPONENT_ADD_INCLUDEDIRS := \
    examples/event_group \
    examples/isr \
    examples/mutex \
    examples/queue \
    examples/semaphore \
    examples/task_notification
