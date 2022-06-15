#!/bin/bash
BUILD_SPEED=1
NEEDED_PACKAGES="automake autoconf boost boost-libs double-conversion fmt gflags git google-glog gperf gtest jsoncpp libevent libsodium lz4 openssl python snappy sqlite zstd zlib ninja cmake libtool xz openssl"

function install_repo_deps () {
    sudo pacman -Syu --needed $NEEDED_PACKAGES
    echo -e "[pacman-ing] : all pacman depedencies have been installed"
}

function install_folly () {
    if [[ ! -f /usr/local/lib/libfolly.a ]]; then
        echo -e "[building] folly"
        git clone https://github.com/windflaag/folly.git
        cd folly/build
        cmake ..
        make -j$BUILD_SPEED
        sudo make install
        cd ../..
        echo -e "[building] folly : installed"
    else
        echo -e "[building] folly : already installed"
    fi
}

function install_fizz () {
    if [[ ! -f /usr/local/lib/libfizz.a ]]; then
        echo -e "[building] fizz"
        git clone https://github.com/windflaag/fizz.git
        mkdir fizz/build_ && cd fizz/build_
        cmake ../fizz
        make -j$BUILD_SPEED
        sudo make install
        cd ../..
        echo -e "[building] fizz : installed"
    else
        echo -e "[building] fizz : already installed"
    fi
}

function install_wangle () {
    if [[ ! -f /usr/local/lib/libwangle.a ]]; then
        echo -e "[building] wangle"
        git clone https://github.com/windflaag/wangle.git
        mkdir wangle/build_ && cd wangle/build_
        cmake ../wangle
        make -j$BUILD_SPEED
        sudo make install
        cd ../..
        echo -e "[building] wangle : installed"
    else
        echo -e "[building] wangle : already installed"
    fi
}

function install_mvfst () {
    if [[ -z "$(find /usr/local/lib -name 'libmvfst*.a')" ]]; then
        echo -e "[building] mvfst"
        git clone https://github.com/windflaag/mvfst.git
        mkdir mvfst/build_ && cd mvfst/build_
        cmake ..
        make -j$BUILD_SPEED
        sudo make install
        cd ../..
        echo -e "[building] mvfst : installed"
    else
        echo -e "[building] mvfst : already installed"
    fi
}

function install_proxygen () {
    if [[ ! -f /usr/local/lib/libproxygen.a ]]; then
        echo -e "[building] proxygen"
        git clone https://github.com/windflaag/proxygen.git
        mkdir proxygen/build_ && cd proxygen/build_
        cmake ..
        make -j$BUILD_SPEED
        sudo make install
        cd ../..
        echo -e "[building] proxygen : installed"
    else
        echo -e "[building] proxygen : already installed"
    fi
}

function install_git_deps () {
    install_folly
    install_fizz
    install_wangle
    install_mvfst
    install_proxygen
}

function install_dependecies () {
    install_repo_deps
    install_git_deps
}

function main () {
    rm -rf deps && mkdir deps && cd deps
    install_dependecies
    cd ..
}

main
