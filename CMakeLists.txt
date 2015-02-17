# This file is part of the TSC Settingseditor.
# Copyright © 2015 Marvin Gülker
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)
enable_language(CXX)
project(TSC_Settingseditor)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
include(ExternalProject)

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
if (NOT(TSC_SETTINGSEDITOR_VERSION_GIT))
  unset(TSC_SETTINGSEDITOR_VERSION_GIT)
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
# External project builds

# inih
set(inih_sources "inih/ini.c" "inih/ini.h" "inih/cpp/INIReader.cpp" "inih/cpp/INIReader.h")
include_directories(${CMAKE_SOURCE_DIR}/inih ${CMAKE_SOURCE_DIR}/inih/cpp)
add_library(inih STATIC ${inih_sources})

# Pathie
ExternalProject_Add(
  pathie
  PREFIX "${CMAKE_BINARY_DIR}/pathie"
  DOWNLOAD_COMMAND ""
  UPDATE_COMMAND ""
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/pathie/install"
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/pathie")
include_directories(${CMAKE_BINARY_DIR}/pathie/install/include/pathie)

########################################
# Dependencies

#find_package(PkgConfig REQUIRED)
find_package(wxWidgets 3.0.0 REQUIRED xrc adv core base)

include(${wxWidgets_USE_FILE})
include_directories(${wxWidgets_INCLUDE_DIRS})

########################################
# Source files

file(GLOB_RECURSE editor_sources
  "src/*.cpp"
  "src/*.hpp")

########################################
# Targets

add_executable(tsc-settingseditor ${editor_sources})
target_link_libraries(tsc-settingseditor ${wxWidgets_LIBRARIES} ${CMAKE_BINARY_DIR}/pathie/install/lib/libpathie.a inih)
add_dependencies(tsc-settingseditor pathie)

########################################
# Installation information

install(TARGETS tsc-settingseditor
  DESTINATION bin)
