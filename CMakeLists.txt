cmake_minimum_required(VERSION 2.8)
enable_language(CXX)
project(Pathie)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

########################################
# Version number

set(TSC_SETTINGSEDITOR_VERSION_MAJOR 0)
set(TSC_SETTINGSEDITOR_VERSION_MINOR 0)
set(TSC_SETTINGSEDITOR_VERSION_PATCH 0)

execute_process(COMMAND git rev-parse --short HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE TSC_SETTINGSEDITOR_VERSION_GIT
  OUTPUT_STRIP_TRAILING_WHITESPACE)

# If git was not available, unset empty variable
if (NOT(PATHIE_VERSION_GIT))
  unset(PATHIE_VERSION_GIT)
endif()

########################################
# Extra flags

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
endif()

# We only support Vista upwards. See http://msdn.microsoft.com/en-us/library/windows/desktop/aa383745(v=vs.85).aspx
# for the macro values.
if (WIN32)
  add_definitions("-D_WIN32_WINNT=0x0600") # Vista
  add_definitions("-D_WIN32_IE=0x0800") # IE 8.0+
  add_definitions("-DWINVER=0x0600") # Vista
endif()

########################################
# Dependencies

#find_package(PkgConfig REQUIRED)
find_package(wxWidgets 3.0.0 REQUIRED xrc core base)
find_package(Pathie REQUIRED)

include(${wxWidgets_USE_FILE})
include_directories(${wxWidgets_INCLUDE_DIRS})
include_directories(${Pathie_INCLUDE_DIR})

########################################
# Source files

file(GLOB_RECURSE editor_sources
  "src/*.cpp"
  "src/*.hpp")
set(inih_sources "inih/ini.c" "inih/ini.h" "inih/cpp/INIReader.cpp" "inih/cpp/INIReader.h")

include_directories(${CMAKE_SOURCE_DIR}/inih ${CMAKE_SOURCE_DIR}/inih/cpp)

########################################
# Targets

add_library(inih STATIC ${inih_sources})
add_executable(tsc-settingseditor ${editor_sources})
target_link_libraries(tsc-settingseditor ${wxWidgets_LIBRARIES} ${Pathie_LIBRARIES} inih)

########################################
# Installation information

install(TARGETS tsc-settingseditor
  DESTINATION bin)
