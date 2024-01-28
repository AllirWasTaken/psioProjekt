cmake_minimum_required(VERSION 3.27)
project(ImageRecognition)

set(CMAKE_CXX_STANDARD 23)

set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/opencv2Libs)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


add_executable(ImageRecognition
        common/async/ThreadPool.cpp
        detection/ObjectDetection.cpp
        input/Command.cpp
        input/CommandLine.cpp
        network/CameraConnection.cpp
        ui/Display.cpp
        ImageDetection.cpp
        ImageRecognition.cpp
        main.cpp
)
target_link_libraries (ImageRecognition ${OpenCV_LIBS})
target_include_directories(ImageRecognition PRIVATE .)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")