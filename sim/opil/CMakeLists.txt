cmake_minimum_required(VERSION 3.18)

add_library(opil)

# Opil core
target_sources(
    opil
    PRIVATE
    opiltarget.c
    opilhost.c
)

# Controller interface
target_sources(
    opil
    PRIVATE
    ctlrif/ctlrif.c
)

# Communication
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(
        opil
        PRIVATE
        comm/linux/hostCommSock.c
        comm/linux/targetCommSock.c
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(
        opil
        PRIVATE
        comm/win/hostCommSock.c
        comm/win/targetCommSock.c
    )
    target_link_libraries(opil PRIVATE ws2_32)
else ()
    message("-- OPIL: platform ${CMAKE_SYSTEM_NAME} not supported.")
endif ()

target_include_directories(
    opil
    PRIVATE
    ${PROJECT_SOURCE_DIR}/
    comm/
)

target_include_directories(
    opil
    PUBLIC
    ./
)
