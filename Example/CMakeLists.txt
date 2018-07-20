cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-fopenmp -fpermissive")

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(mySegmentationProgram mySegmentationProgram.cpp ../itkRWSegmentationFilter/itkRWSegmentationFilter.h )
target_link_libraries(mySegmentationProgram ${ITK_LIBRARIES} )