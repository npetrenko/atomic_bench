#!/bin/bash

mkdir release

cd release && \
    cmake -DCMAKE_BUILD_TYPE=RELEASE .. && \
    make && ./bench && cd ..
