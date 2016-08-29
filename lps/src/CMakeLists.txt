cmake_minimum_required(VERSION 2.8)

add_definitions(-std=c++11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_LIBRARY_PATH /usr/local/lib)
set(CMAKE_INCLUDE_PATH /usr/local/include)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_executable(lps main.cpp statsd-client-cpp/src/statsd_client.cpp arucodrone/arucodrone.cpp arucodrone/cameralocation.cpp arucodrone/commands.cpp arucodrone/detect.cpp arucodrone/flyto.cpp arucodrone/markerlocation.cpp arucodrone/pid.cpp ar_drone/ardrone/ardrone.cpp ar_drone/ardrone/command.cpp ar_drone/ardrone/config.cpp ar_drone/ardrone/navdata.cpp ar_drone/ardrone/tcp.cpp ar_drone/ardrone/udp.cpp ar_drone/ardrone/version.cpp ar_drone/ardrone/video.cpp)

target_link_libraries(lps -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab -lswscale -lavutil -lavformat -lavcodec -lavdevice -lavfilter -laruco -lraspicam -lraspicam_cv -lm -lpthread -lrt -lpthread)
