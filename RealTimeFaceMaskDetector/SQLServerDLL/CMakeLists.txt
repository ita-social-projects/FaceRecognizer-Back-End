cmake_minimum_required(VERSION 3.15)
project(SQLServerDLL)

include_directories(C:/SQLAPI/include)
link_directories(C:/SQLAPI/vs2019/x86_64/lib)

set(SOURCES SQLConnection.h
        SQLException.h SQLException.cpp
        SQLServer.h SQLServer.cpp
        XMLParser.h XMLParser.cpp
        IniParser.h IniParser.cpp
        pch.h pch.cpp
        framework.h
        ConfigReader.h)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_compile_definitions(SQLSERVER_EXPORTS)

add_library(SQLServerDLL SHARED dllmain.cpp ${SOURCES})

#set_target_properties(SQLServerDLL PROPERTIES RUNTIME_OUTPUT_DIRECTORY
#        ${PROJECT_BINARY_DIR}/build_results/bin )
#set_target_properties( mylib PROPERTIES LIBRARY_OUTPUT_DIRECTORY)

target_link_libraries(SQLServerDLL sqlapi)


