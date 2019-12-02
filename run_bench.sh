#!/bin/bash

mkdir release

BENCH_NAME=$1

cd release && \
    cmake -DCMAKE_BUILD_TYPE=RELEASE .. && \
    cd $BENCH_NAME &&
    make && ./$BENCH_NAME && cd .. && cd ..
