# main interface library -- precison.hpp and stdafx.h
set(LIBRARY_NAME main)

# create interface
add_library(${LIBRARY_NAME} INTERFACE)

# publish headers for this library
target_include_directories(${LIBRARY_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# set compile flags
target_compile_definitions(${LIBRARY_NAME} INTERFACE PRECISION=${PRECISION})

# main executable - FastSim
set(EXECUTABLE FastSim)

# source files
set(SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# create executable
add_executable(${EXECUTABLE} ${SOURCE_FILES})

# dependencies
target_link_libraries(${EXECUTABLE}
        Approximations
        ${FFTW_LIB}
)