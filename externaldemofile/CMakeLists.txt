# ------ Set the basic options for the demo project ------- #
message(STATUS "# Set the basic options for the external demo project")

cmake_minimum_required(VERSION 3.20)
project(ExtDemofile LANGUAGES CXX)

# ------ Create the executable program ------ #
if (WIN32)
    message(STATUS "- Demo Platform : Windows")
    add_executable(${PROJECT_NAME} WIN32)
else()
    message(STATUS "- Demo Platform : Others")
    add_executable(${PROJECT_NAME})
endif()

# ------ Set the project options ------ #
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Project.cmake)
project_options()

# ------ Load the extenral project ------ #
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/ExternalProject.cmake)

# ------ Load the log project ------ #
add_external_library(
    PROJECT_NAME LogProject_Ext
    LIBRARY_NAME LogProject
    GIT_REPOSITORY https://github.com/Winteradio/Log.git
    BUILD_SHARED_LIBS ON
)

# ------ Load the reflection project ------ #
add_external_library(
    PROJECT_NAME ReflectionProject_Ext
    LIBRARY_NAME ReflectionProject
    GIT_REPOSITORY https://github.com/Winteradio/Reflection.git
    GIT_TAG CMAKE_TEST
    BUILD_SHARED_LIBS ON
)

# ------ Set the properties the executable program ------ #
target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)