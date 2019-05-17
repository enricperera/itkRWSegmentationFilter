cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_CXX_FLAGS "-fopenmp -fpermissive -Wno-error=return-type -Wno-error=unused-but-set-variable -Wno-error=unused-value -Wno-error=unused-variable -Wno-error=cpp -Wno-error=narrowing -Wno-error=sign-compare")

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(cpuExample MACOSX_BUNDLE cpuExample.cpp ../itkRWSegmentationFilter/itkRWSegmentationFilter.h )
target_link_libraries(cpuExample ${ITK_LIBRARIES} )

find_package(CUDA)

if (CUDA_FOUND)
    message(STATUS "--CUDA found. Building GPU example.")

    SET(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -arch=sm_30 -std=c++11)
    SET(CUDA_SEPARABLE_COMPILATION ON)
    SET(CUDA_HOST_COMPILATION_CPP ON)
    SET(USECUBLAS ON)

    add_executable(gpuExample MACOSX_BUNDLE gpuExample.cpp ../itkRWSegmentationFilter/itkRWSegmentationFilter.h )
    target_link_libraries(gpuExample ${ITK_LIBRARIES} ${CUDA_LIBRARIES} ${CUDA_cublas_LIBRARY} ${CUDA_cusparse_LIBRARY})
else()
    message(STATUS "--CUDA not found. Skiping build of GPU example.")
endif()