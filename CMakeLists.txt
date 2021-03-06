cmake_minimum_required(VERSION 3.5)

project(computed_tomography_task LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(OpenCV REQUIRED)

FIND_PATH(MathGL_INCLUDE_DIRS NAMES mgl2/mgl.h
  PATHS
  /usr/include
  /usr/local/include
)

FIND_PATH(MathGL_INCLUDE_DIRS NAMES mgl2/qt.h
  PATHS
  /usr/include
  /usr/local/include
)





FIND_LIBRARY(MathGL_LIB NAMES mgl
  PATHS
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(MathGL_LIB NAMES mgl-qt5
  PATHS
  /usr/local/lib
  /usr/lib
)



SET(MathGL_LIBRARIES ${MathGL_LIB} )

IF (MathGL_INCLUDE_DIRS AND MathGL_LIBRARIES)
  SET(MathGL_FOUND TRUE)
  MESSAGE(STATUS "MathGL found")
  MESSAGE(STATUS "MathGL Include dirs:" ${MathGL_INCLUDE_DIRS})
  MESSAGE(STATUS "MathGL Library:" ${MathGL_LIB})
ELSE (MathGL_INCLUDE_DIRS AND MathGL_LIBRARIES)
  MESSAGE(STATUS "MathGL was not found")
ENDIF(MathGL_INCLUDE_DIRS AND MathGL_LIBRARIES)


include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${MATHGL2_INCLUDE_DIRS})

link_directories(${OpenCV_LIB_DIR})
link_directories(${CMAKE_PREFIX_PATH}/lib/)


set(HDR
    algebraic_solution.h
    algotools.h
    algorithms.h
    cluster.h
    determinant.h
    detector_interface.h
    dispersion_detector.h
    lines.h
    matrix.h
    matrix_utils.h
    points.h
    utils.h
    )

set(SRC
    algebraic_solution.cpp
    algotools.cpp
    algorithms.cpp
    cluster.cpp
    determinant.cpp
    dispersion_detector.cpp
    lines.cpp
    matrix.cpp
    matrix_utils.cpp
    points.cpp
    utils.cpp
    )

#test of opencv posibilites
add_executable(test_opencv  test_opencv_main.cpp)
target_link_libraries(test_opencv ${OpenCV_LIBS})

#test of MathGL posibilities
add_executable(test_mathgl test_mathgl_main.cpp)
target_link_libraries(test_mathgl mgl mgl-qt5 )

#test of MathGL draw plot in picture
add_executable(test_mathgl_picture test_mathgl_picture_main.cpp)
target_link_libraries(test_mathgl_picture mgl  )


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3  -fPIC -pthread -fsanitize=bounds -fsanitize=address ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3  -fPIC -pthread -fsanitize=bounds -fsanitize=address ")



#task
add_executable(computed_tomography_task  ${HDR} ${SRC} main.cpp)
target_link_libraries(computed_tomography_task mgl ${OpenCV_LIBS})
#target_compile_options(computed_tomography_task PRIVATE "SHELL:--param ggc-min-expand=1")
#target_compile_options(computed_tomography_task PRIVATE "SHELL:--param ggc-min-heapsize=262144")

