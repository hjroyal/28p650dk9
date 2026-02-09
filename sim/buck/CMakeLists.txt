cmake_minimum_required(VERSION 3.18)

project(opil-buck-example LANGUAGES C)

set(CMAKE_C_STANDARD 11)

# Option to build PLECS DLL
option(BUILD_PLECS_DLL "Build PLECS DLL (libopil_plecs)" OFF)

# Determine platform
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OPIL_PLATFORM "win")
    set(SOCKET_LIB ws2_32)
    set(SHARED_LIB_EXT ".dll")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(OPIL_PLATFORM "linux")
    set(SHARED_LIB_EXT ".so")
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

# Include directories for target
include_directories(
    ${CMAKE_SOURCE_DIR}/../..  # For opil headers
    ${CMAKE_SOURCE_DIR}/../../comm
    ${CMAKE_SOURCE_DIR}/../../simif
    ${CMAKE_SOURCE_DIR}/../../ctlrif
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}  # For stypes.h
)

# Source files for target executable
set(TARGET_COMMON_SOURCES
    ${CMAKE_SOURCE_DIR}/../../ctlrif/ctlrif.c
    ${CMAKE_SOURCE_DIR}/../../opiltarget.c
    ${CMAKE_SOURCE_DIR}/src/buckcontrol.c
    ${CMAKE_SOURCE_DIR}/src/main.c
)

# Platform-specific communication source for target
if(OPIL_PLATFORM STREQUAL "win")
    set(TARGET_COMM_SOURCES ${CMAKE_SOURCE_DIR}/../../comm/win/targetCommSock.c)
elseif(OPIL_PLATFORM STREQUAL "linux")
    set(TARGET_COMM_SOURCES ${CMAKE_SOURCE_DIR}/../../comm/linux/targetCommSock.c)
endif()

# All sources for target
set(TARGET_ALL_SOURCES ${TARGET_COMMON_SOURCES} ${TARGET_COMM_SOURCES})

# Define target executable
add_executable(opiltarget ${TARGET_ALL_SOURCES})

# Target-specific compile definitions
target_compile_definitions(opiltarget 
    PRIVATE 
        TARGET_COMM_SOCK_SERVER_PORT=8090
)

# Link libraries for target
if(OPIL_PLATFORM STREQUAL "win")
    target_link_libraries(opiltarget ${SOCKET_LIB})
endif()

# Set output directory for target
set_target_properties(opiltarget PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Build PLECS DLL if requested
if(BUILD_PLECS_DLL)
    # Include directories for PLECS DLL
    include_directories(
        ${CMAKE_SOURCE_DIR}/../../sw/plecs
    )
    
    # Platform-specific communication source for host
    if(OPIL_PLATFORM STREQUAL "win")
        set(HOST_COMM_SOURCES ${CMAKE_SOURCE_DIR}/../../comm/win/hostCommSock.c)
    elseif(OPIL_PLATFORM STREQUAL "linux")
        set(HOST_COMM_SOURCES ${CMAKE_SOURCE_DIR}/../../comm/linux/hostCommSock.c)
    endif()
    
    # Sources for PLECS DLL
    set(PLECS_SOURCES
        ${CMAKE_SOURCE_DIR}/../../sw/plecs/opil_plecs.c
        ${CMAKE_SOURCE_DIR}/../../simif/simif.c
        ${CMAKE_SOURCE_DIR}/../../opilhost.c
        ${HOST_COMM_SOURCES}
    )
    
    # Add shared library for PLECS
    add_library(opil_plecs SHARED ${PLECS_SOURCES})
    
    # Position independent code for shared library
    set_target_properties(opil_plecs PROPERTIES
        POSITION_INDEPENDENT_CODE TRUE
        OUTPUT_NAME "libopil_plecs"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    # Compile definitions for host communication
    target_compile_definitions(opil_plecs
        PRIVATE
            HOST_COMM_SOCK_SERVER_IP="127.0.0.1"
            HOST_COMM_SOCK_SERVER_PORT=8090
    )
    
    # Link libraries for PLECS DLL
    if(OPIL_PLATFORM STREQUAL "win")
        target_link_libraries(opil_plecs ${SOCKET_LIB})
    endif()
endif()