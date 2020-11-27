project(message)

set(LIBRARY_OUTPUT_PATH /home/ruanjian/workspace/project_pre/message/output/release/lib)

aux_source_directory(. DIR_LIB_SRCS)

add_library(message ${DIR_LIB_SRCS})


