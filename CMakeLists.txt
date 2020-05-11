cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# Set up the project
project(levelx
    VERSION 6.0.0 
    LANGUAGES C ASM
)

if(NOT DEFINED THREADX_ARCH)
    message(FATAL_ERROR "Error: THREADX_ARCH not defined")
endif()
if(NOT DEFINED THREADX_TOOLCHAIN)
    message(FATAL_ERROR "Error: THREADX_TOOLCHAIN not defined")
endif()


# Define our target library and an alias for consumers
add_library(${PROJECT_NAME})
add_library("azrtos::${PROJECT_NAME}" ALIAS ${PROJECT_NAME})

# Define any required dependencies between this library and others
target_link_libraries(${PROJECT_NAME} PUBLIC 
    "azrtos::threadx"
)

# Pick up the common stuff
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/common)

