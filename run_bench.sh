#!/bin/bash

BENCH_NAME=${1%/}
START_DIR=`pwd`

run_compiler() {
    rm -rf release
    mkdir release
    cd release && \
        cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_CXX_COMPILER=$1 .. && \
        cd $BENCH_NAME &&
        make && ./$BENCH_NAME

    cd $START_DIR
    rm -rf release
}

run_compiler g++
run_compiler clang++
