#!/bin/bash
BUILD_SPEED=1

function build_executable () {
    rm -rf ./build
    mkdir ./build
    cd ./build
    cmake ../source
    make
    cd ..
}

function link_to_runtime () {
    cp ./build/main ./runtime/server
}

function main () {
    clear
    build_executable
    link_to_runtime
}

main
