# Every example lives in its own folder under examples/<name>/
file(GLOB EXAMPLE_SRCS "examples/*/*.c")
file(GLOB EXAMPLE_DIRS LIST_DIRECTORIES true "examples/*")

idf_component_register(SRCS "main.c" ${EXAMPLE_SRCS}
                    INCLUDE_DIRS "" ${EXAMPLE_DIRS})
