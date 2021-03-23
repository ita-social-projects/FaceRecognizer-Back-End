cmake_minimum_required(VERSION 3.15)
project(DateTimeDLL)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_compile_definitions(DATETIMEDLL_EXPORTS)

set(SOURCES dllmain.cpp
            DateTime.h DateTime.cpp
            DateTimeException.h DateTimeException.cpp
            framework.h
            pch.h pch.cpp)

add_library(DateTimeDLL SHARED ${SOURCES})
