file(GLOB_RECURSE EXAMPLE ./   *.cpp  )

add_library(example_lib  ${EXAMPLE})