cmake_minimum_required(VERSION 3.15)
project(TCPClientUI)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "C:/Qt_vs/lib/cmake")
set(QtCmake "C:/Qt_vs/lib/cmake")
set(Qt5Widgets_DIR "${QtCmake}/Qt5Widgets")
set(Qt5Dbus_DIR ${QtCmake}/Qt5Dbus)
set(Qt5Core_DIR ${QtCmake}/Qt5Core)
set(Qt5Concurrent_DIR ${QtCmake}/Qt5Concurrent)
set(Qt5Gui_DIR ${QtCmake}/Qt5Gui)

# qt5
find_package(Qt5 COMPONENTS Widgets Gui Core Concurrent REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR "YES")
set(CMAKE_AUTOMOC "YES")
set(CMAKE_AUTOUIC "YES")

# sqlapi
include_directories(C:/SQLAPI/include)
link_directories(C:/SQLAPI/vs2019/x86_64/lib)

# opencv
include_directories(../../3rdPartyLibs/opencv/include/)
link_directories(../../3rdPartyLibs/opencv/lib/)

# sqlserverdll
include_directories(../SQLServerDLL)
link_directories(../../cmake-build-release/RealTimeFaceMaskDetector/SQLServerDLL)

set(SOURCES main.cpp
            FaceRecognitionUI.h FaceRecognitionUI.cpp FaceRecognitionUI.ui
            FaceRecognizer.h FaceRecognizer.cpp
            ImageData.h ImageData.cpp
            TCPClient.h TCPClient.cpp
            TCPClientUI.h TCPClientUI.cpp TCPClientUI.ui TCPClientUI.qrc
            TimeCounting.h)

set(OPENCV_LIBS opencv_core450.lib
        opencv_calib3d450.lib
        opencv_features2d450.lib
        opencv_flann450.lib
        opencv_highgui450.lib
        opencv_imgcodecs450.lib
        opencv_imgproc450.lib
        opencv_ml450.lib
        opencv_objdetect450.lib
        opencv_photo450.lib
        opencv_stitching450.lib
        opencv_video450.lib
        opencv_videoio450.lib)

set(QT5_LIBS Qt5::Widgets Qt5::Concurrent Qt5::Core Qt5::Gui)

add_executable(TCPClientUI ${SOURCES})

target_link_libraries(TCPClientUI ${QT5_LIBS} ${OPENCV_LIBS} sqlapi SQLServerDLL)

# copying dlls into executable file directory
set(Qt5Directory "C:/Qt_vs/")
set(Qt5Bin "${Qt5Directory}/bin")
set(OpenCvBin "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdPartyLibs/opencv/bin")

set(Qt5Dll_PATHS
        ${Qt5Bin}/Qt5Core.dll
        ${Qt5Bin}/Qt5Gui.dll
        ${Qt5Bin}/Qt5Widgets.dll
        ${Qt5Bin}/Qt5Dbus.dll
        ${Qt5Bin}/Qt5Concurrent.dll)

set(OpenCvDll_PATHS
        ${OpenCvBin}/opencv_core450.dll
        ${OpenCvBin}/opencv_calib3d450.dll
        ${OpenCvBin}/opencv_features2d450.dll
        ${OpenCvBin}/opencv_flann450.dll
        ${OpenCvBin}/opencv_highgui450.dll
        ${OpenCvBin}/opencv_imgcodecs450.dll
        ${OpenCvBin}/opencv_imgproc450.dll
        ${OpenCvBin}/opencv_ml450.dll
        ${OpenCvBin}/opencv_objdetect450.dll
        ${OpenCvBin}/opencv_photo450.dll
        ${OpenCvBin}/opencv_stitching450.dll
        ${OpenCvBin}/opencv_video450.dll
        ${OpenCvBin}/opencv_video450.dll
        ${OpenCvBin}/opencv_videoio450.dll)

set(THIRD_PARTY_LIBS
        ${Qt5Dll_PATHS}
        ${OpenCvDll_PATHS}
        ${CMAKE_BINARY_DIR}/RealTimeFaceMaskDetector/SQLServerDLL/SQLServerDLL.dll
        C:/SQLAPI/vs2019/x86_64/bin/sqlapi.dll
        )
foreach (libpath ${THIRD_PARTY_LIBS})
    add_custom_command(TARGET TCPClientUI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${libpath}"
            "${CMAKE_BINARY_DIR}/RealTimeFaceMaskDetector/TCPClientUI")
endforeach ()