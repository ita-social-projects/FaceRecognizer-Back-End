cmake_minimum_required(VERSION 3.15)
project(WinService)
set(CMAKE_CXX_STANDARD 20)

include_directories(../Logger)
link_directories(../../cmake-build-release/RealTimeFaceMaskDetector/Logger)
link_directories(C:/SQLAPI/vs2019/x86_64/lib)

include_directories(../SQLServerDLL)
link_directories(../../cmake-build-release/RealTimeFaceMaskDetector/SQLServerDLL)

set(SOURCES Main.cpp
        WindowsService.h WindowsService.cpp)

add_executable(WinService ${SOURCES})

target_link_libraries(WinService sqlapi)
target_link_libraries(WinService LoggerDLL)
target_link_libraries(WinService SQLServerDLL)

set(THIRD_PARTY_LIBS
        C:/SQLAPI/vs2019/x86_64/bin/sqlapi.dll
        ${CMAKE_BINARY_DIR}/RealTimeFaceMaskDetector/SQLServerDLL/SQLServerDLL.dll
        )
foreach (libpath ${THIRD_PARTY_LIBS})
    add_custom_command(TARGET WinService POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${libpath}"
            "${CMAKE_BINARY_DIR}/RealTimeFaceMaskDetector/WinService")
endforeach ()
