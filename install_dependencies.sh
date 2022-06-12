#!/bin/bash
BUILD_SPEED=1

function install_repo_deps () {
    pacman -Q | awk '{ print $1 }' > installed_packages.txt
    INSTALL_QUEUE_AS_COLUMN="$(comm -13  <(sort installed_packages.txt) <(sort needed_packages.txt))"
    INSTALL_QUEUE="$(echo "$INSTALL_QUEUE_AS_COLUMN" | awk 'BEGIN { ORS = " " } { print }' | xargs)"
    if [[ "x"$INSTALL_QUEUE != "x" ]]; then
        echo "sudo pacman -Syu $INSTALL_QUEUE"
    else
        echo -e "nothing to install with pacman"
    fi
}

function install_folly () {
    if [[ ! -f /usr/local/lib/libfolly.a ]]; then
        exit 1
        git clone https://github.com/windflaag/folly.git
        cd folly/build
        cmake ..
        make -j$BUILD_SPEED
        sudo make install
        cd ../..
    else
        echo -e "folly already installed"
    fi
}

function install_fizz () {
    if [[ ! -f /usr/local/lib/libfizz.a ]]; then
        exit 1
        git clone https://github.com/windflaag/fizz.git
        mkdir fizz/build_ && cd fizz/build_
        cmake ../fizz
        make -j$BUILD_SPEED
        sudo make install
        cd ../..
    else
        echo -e "fizz already installed"
    fi
}

function install_wangle () {
    exit 0
    if [[ ! -f /usr/local/lib/libwangle.a ]]; then
        exit 1
        git clone https://github.com/windflaag/wangle.git
        mkdir wangle/build_ && cd wangle/build_
        cmake ../wangle
        make -j$BUILD_SPEED
        sudo make install
        cd ../..
    else
        echo -e "wangle already installed"
    fi
}

function install_mvfst () {
    exit 0
    if [[ ! -z "$(find /usr/local/lib -name 'libmvfst*.a')" ]]; then
        exit 1
        git clone https://github.com/windflaag/mvfst.git
        mkdir mvfst/build_ && cd mvfst/build_
        cmake ..
        make -j$BUILD_SPEED
        sudo make install
        cd ../..
    else
        echo -e "mvfst already installed"
    fi
}

function install_proxygen () {
    exit 0
    if [[ ! -f /usr/local/lib/libproxygen.a ]]; then
        exit 1
        git clone https://github.com/windflaag/proxygen.git
        mkdir proxygen/build_ && cd proxygen/build_
        cmake ..
        make -j$BUILD_SPEED
        sudo make install
        cd ../..
    else
        echo -e "proxygen already installed"
    fi
}

function collect_git_deps () {
    install_folly
    install_fizz
    install_wangle
    install_mvfst
    install_proxygen
}

function install_git_deps () {
    collect_git_deps
}

function install_dependecies () {
    install_repo_deps
    install_git_deps
}

function main () {
    install_dependecies
}

main
