cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(PCL_BUNDLER_READER)

find_package(PCL 1.3 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(pclBundlerReader pointcloudscene.cpp camera.cpp)
target_link_libraries(pclBundlerReader ${PCL_LIBRARIES})
