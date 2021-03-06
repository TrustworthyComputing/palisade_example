cmake_minimum_required (VERSION 3.5.1)

project(demo C CXX)
set(CMAKE_CXX_STANDARD 11)

if(APPLE)
        set(OPENMP_LIBRARIES "/usr/local/opt/libomp/lib")
        set(OPENMP_INCLUDES "/usr/local/opt/libomp/include")
endif()

OPTION (USE_OpenMP "Use OpenMP to enable <omp.h>" ON)

# Set OpenMP configuration manually for macOS
if(APPLE AND USE_OpenMP)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
       set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -lomp -Wno-unused-command-line-argument")
       set(OpenMP_C_LIB_NAMES "libomp")
       set(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
       set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -lomp -Wno-unused-command-line-argument")
       set(OpenMP_CXX_LIB_NAMES "libomp")
       set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
    endif()
endif()

find_package (OpenMP)

if( APPLE AND USE_OpenMP)
        include_directories("${OPENMP_INCLUDES}")
        link_directories("${OPENMP_LIBRARIES}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif()

# OpenMP_CXX_FOUND was added in cmake 3.9.x
# so we are also checking the OpenMP_FOUND flag
if (OpenMP_CXX_FOUND OR OpenMP_FOUND)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
        message("-- OpenMP is turned ON")
else()
        message("-- OpenMP is turned OFF")
endif()

if (OpenMP_C_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif()

# Added -Wno-parentheses -Wno-pessimizing-move for compatibility with g++ >= v9.0
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
                set (IGNORE_WARNINGS "")
        else()
                set (IGNORE_WARNINGS "-Wno-parentheses -Wno-pessimizing-move")
        endif()
endif()
if(APPLE AND "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        set(IGNORE_WARNINGS "${IGNORE_WARNINGS} -Wno-unused-private-field -Wno-shift-op-parentheses")
endif()

set(COMPILEFLAGS "-Wall -Werror -O3 -DPALISADE_VERSION=${PALISADE_VERSION} ${IGNORE_WARNINGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMPILEFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILEFLAGS}")

# be sure to set PALISADE_DIR to the base install directory for PALISADE: -DPALISADE_DIR=/your/path

if( "${PALISADE_DIR}" STREQUAL "" )
	message(SEND_ERROR "PALISADE_DIR must be set to the install location for PALISADE")
endif()

include_directories( ${PALISADE_DIR}/include/palisade )
include_directories( ${PALISADE_DIR}/include/palisade/core )
include_directories( ${PALISADE_DIR}/include/palisade/pke )

link_directories( ${PALISADE_DIR}/lib )

link_libraries( PALISADEcore PALISADEpke ${OpenMP_CXX_FLAGS} )

### if using NTL include the next two lines, else don't
#link_libraries( gmp ntl )
#add_definitions(-DWITH_NTL)

### ADD YOUR EXECUTABLE(s) HERE
### add_executable( EXECUTABLE-NAME SOURCES )
###
### EXAMPLE:
### add_executable( test mult-demo.cpp )

add_executable( mult-demo mult-demo.cpp )

