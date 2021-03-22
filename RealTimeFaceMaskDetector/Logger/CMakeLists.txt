cmake_minimum_required(VERSION 3.15)
project(LoggerDLL)
set(CMAKE_CXX_STANDARD 17)

set(SOURCES Logger.h Logger.cpp
            pch.h pch.cpp
            framework.h)
set(THIRD_PARTY_SOURCES easylogging++.h easylogging++.cpp)

add_library(LoggerDLL STATIC ${THIRD_PARTY_SOURCES} ${SOURCES})