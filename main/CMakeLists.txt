file(GLOB EXAMPLE_SRCS "examples/*.c")

idf_component_register(SRCS "main.c" ${EXAMPLE_SRCS}
                    INCLUDE_DIRS "" "examples")
