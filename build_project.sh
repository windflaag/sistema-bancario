#!/bin/bash
BUILD_SPEED=$(nproc --ignore=1)

CMAKE_C_COMPILER=gcc
CMAKE_CXX_COMPILER=g++
CMAKE_GENERATOR=""
MAKE_COMMAND="make"
MAKE_COMMAND_ARGS="-j$BUILD_SPEED"

USE_CLANG_IF_EXISTS=no
USE_NINJA_IF_EXISTS=no

function check_clang () {
    if [[ -f /usr/bin/clang++ && $USE_CLANG_IF_EXISTS == yes ]]; then
        CMAKE_C_COMPILER=clang
        CMAKE_CXX_COMPILER=clang++
        echo -e "using clang"
    fi
}

function check_ninja () {
    if [[ -f /usr/bin/ninja && $USE_NINJA_IF_EXISTS == yes ]]; then
        CMAKE_GENERATOR="-G Ninja"
        MAKE_COMMAND="ninja"
        echo -e "using ninja"
    fi
}

function build_executable () {
    rm -rf ./build
    mkdir ./build
    cd ./build

    LAUNCHER_CMAKE="cmake ../source -D CMAKE_C_COMPILER=$CMAKE_C_COMPILER -D CMAKE_CXX_COMPILER=$CMAKE_CXX_COMPILER $CMAKE_GENERATOR"
    LAUNCHER_MAKE="$MAKE_COMMAND $MAKE_COMMAND_ARGS"

    $LAUNCHER_CMAKE && $LAUNCHER_MAKE
}

function main () {
    clear
    check_clang
    check_ninja
    build_executable
}

main
