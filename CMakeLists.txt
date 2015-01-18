cmake_minimum_required(VERSION 2.8)

project (cpp_project)

file(
    GLOB_RECURSE
    SERVER_SRC
    Server/Src/*
    Src/*
)

file(
    GLOB_RECURSE
    CLIENT_SRC
    Client/Src/*
    Src/*
)

SET( LIBGWS_SRV_INC_DIR ./Server/Inc)
SET( LIBGWS_CLI_INC_DIR ./Client/Inc)
SET( LIBGWS_INC_DIR ./Inc)

INCLUDE_DIRECTORIES(
    ${LIBGWS_SRV_INC_DIR}
    ${LIBGWS_CLI_INC_DIR}
    ${LIBGWS_INC_DIR}
)
add_executable(server ${SERVER_SRC})
add_executable(client ${CLIENT_SRC})
SET(
    EXTRA_LIBS
    pthread
)

target_link_libraries( server ${EXTRA_LIBS} )
target_link_libraries( client ${EXTRA_LIBS} ncurses)
