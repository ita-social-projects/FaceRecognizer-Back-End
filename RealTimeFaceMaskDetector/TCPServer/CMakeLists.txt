cmake_minimum_required(VERSION 3.15)
project(TCPServer)
set(CMAKE_CXX_STANDARD 17)

include_directories(../Logger)
link_directories(../../cmake-build-release/RealTimeFaceMaskDetector/Logger)

include_directories(C:/SQLAPI/include)
link_directories(C:/SQLAPI/vs2019/x86_64/lib)

include_directories(../SQLServerDLL)
link_directories(../../cmake-build-release/RealTimeFaceMaskDetector/SQLServerDLL)

set(SOURCES SocketServer.h SocketServer.cpp
        ServiceStarter.h ServiceStarter.cpp
        ThreadPool.h ThreadPool.cpp)

add_executable(TCPServer server_main.cpp ${SOURCES})

target_link_libraries(TCPServer sqlapi)
target_link_libraries(TCPServer LoggerDLL)
target_link_libraries(TCPServer SQLServerDLL)

set(THIRD_PARTY_LIBS
        C:/SQLAPI/vs2019/x86_64/bin/sqlapi.dll
        ${CMAKE_BINARY_DIR}/RealTimeFaceMaskDetector/SQLServerDLL/SQLServerDLL.dll
        )
foreach (libpath ${THIRD_PARTY_LIBS})
    add_custom_command(TARGET TCPServer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${libpath}"
            "${CMAKE_BINARY_DIR}/RealTimeFaceMaskDetector/TCPServer")
endforeach ()