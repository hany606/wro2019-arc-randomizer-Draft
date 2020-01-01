#!/usr/bin/env bash

cd ../generator/

mkdir -p 'cmake-build'

cd 'cmake-build'

echo "Running CMake from $(pwd)"
cmake ..
make