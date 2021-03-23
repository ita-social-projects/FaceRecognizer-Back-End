cmake_minimum_required(VERSION 3.15)
project(LogViewer)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "C:/Qt_vs/lib/cmake")
set(QtCmake "C:/Qt_vs/lib/cmake")
set(Qt5Widgets_DIR "${QtCmake}/Qt5Widgets")
set(Qt5Dbus_DIR ${QtCmake}/Qt5Dbus)
set(Qt5Core_DIR ${QtCmake}/Qt5Core)
set(Qt5Concurrent_DIR ${QtCmake}/Qt5Concurrent)
set(Qt5Gui_DIR ${QtCmake}/Qt5Gui)

include_directories(../DateTimeDLL)
link_directories(${CMAKE_BINARY_DIR}/RealTimeFaceMaskDetector/DateTimeDLL)

find_package(Qt5 COMPONENTS Widgets Gui Core Concurrent REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR "YES")
set(CMAKE_AUTOMOC "YES")
set(CMAKE_AUTOUIC "YES")

set(SOURCES main.cpp
            LogViewer.h LogViewer.cpp LogViewer.ui LogViewer.qrc
            Parser.h Parser.cpp
            Filter.h Filter.cpp
            LogViewerException.h
            LogSearcher.h
            structures.h
            utility.h)

set(QT5_LIBS Qt5::Widgets Qt5::Concurrent Qt5::Core Qt5::Gui)

add_executable(LogViewer ${SOURCES})

target_link_libraries(LogViewer ${QT5_LIBS} DateTimeDLL)

# copying dlls into executable file directory
set(Qt5Directory "C:/Qt_vs/")
set(Qt5Bin "${Qt5Directory}/bin")

set(Qt5Dll_PATHS
        ${Qt5Bin}/Qt5Core.dll
        ${Qt5Bin}/Qt5Gui.dll
        ${Qt5Bin}/Qt5Widgets.dll
        ${Qt5Bin}/Qt5Dbus.dll
        ${Qt5Bin}/Qt5Concurrent.dll)

set(THIRD_PARTY_LIBS
        ${Qt5Dll_PATHS}
        ${CMAKE_BINARY_DIR}/RealTimeFaceMaskDetector/DateTimeDLL/DateTimeDLL.dll
        )
foreach (libpath ${THIRD_PARTY_LIBS})
    add_custom_command(TARGET LogViewer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${libpath}"
            "${CMAKE_BINARY_DIR}/RealTimeFaceMaskDetector/LogViewer")
endforeach ()