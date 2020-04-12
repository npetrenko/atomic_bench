#!/bin/bash

SOURCE_PATH=$(readlink -f $(dirname $0))
echo $SOURCE_PATH

BENCH_NAME=$(basename $1)

STORE_DIR="/tmp"

run_compiler() {
    compiler=$1
    target_dir="${STORE_DIR}/release_${compiler}"

    mkdir $target_dir || true
    (cd $target_dir && \
        cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_CXX_COMPILER=$compiler $SOURCE_PATH && \
        cd $BENCH_NAME && \
        make -j && \
        ./$BENCH_NAME)
}

# run_compiler g++
run_compiler clang++
