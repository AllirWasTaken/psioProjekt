cmake_minimum_required(VERSION 3.26)
project(ImageRecognition)

set(CMAKE_CXX_STANDARD 23)

set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/opencv2Libs)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


add_executable(ImageRecognition
        main.cpp
        Config.cpp
        Config.h
        Image.cpp
        Image.h
        ObjectDetection.cpp
        ObjectDetection.h
        TaskStack.cpp
        TaskStack.h
        StreamReciver.cpp
        StreamReciver.h
        ObjectClassifier.cpp
        ObjectClassifier.h
        Labeler.cpp
        Labeler.h
        Predictor.cpp
        Predictor.h
)
target_link_libraries (ImageRecognition ${OpenCV_LIBS})



set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")