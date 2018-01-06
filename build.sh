#!/bin/bash

set -e

IMAGE=spielhuus/toolchain
TAG=`if [ -z "$1" ]; then echo "master"; else echo "$1" ; fi`
PID=$(sudo docker run -itd -v $(pwd):/repo -v $(pwd)/.build:/.build $IMAGE /bin/sh)
echo "build libavcpp tag:$TAG, image:$PID"

DOCKER_EXEC="sudo docker exec $PID /bin/sh -c"

$DOCKER_EXEC "cd .build && conan install /repo --build=missing"
$DOCKER_EXEC "cmake -H/repo -B/.build -G Ninja -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -Dbuild_tests=on -Dbuild_samples=off -Dbuild_documentation=on  -DLIBAVCPP_VERSION=$TAG"
$DOCKER_EXEC "cmake --build /.build"
$DOCKER_EXEC "cmake --build /.build --target doc"

