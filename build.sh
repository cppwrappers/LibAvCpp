#!/bin/bash

set -e
set -x

IMAGE=spielhuus/toolchain
TAG=`if [ -z "$1" ]; then echo "master"; else echo "$1" ; fi`
PID=$(sudo docker run -itd -v $(pwd):/repo -v $(pwd)/.build:/.build $IMAGE /bin/sh)
echo "build libavcpp tag:$TAG, image:$PID"

DOCKER_EXEC="sudo docker exec $PID /bin/sh -c"

#$DOCKER_EXEC "cd .build && conan install /repo --build=missing"
$DOCKER_EXEC "cmake -H/repo -B/.build -G Ninja -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -Dbuild_tests=on -Dbuild_samples=off -Dbuild_documentation=on  -DLIBAVCPP_VERSION=$TAG"
#$DOCKER_EXEC "cmake --build /.build"
#$DOCKER_EXEC "cmake --build /.build --target doc"

if [ -z "$1" ]; then
        echo not deploying to bintray, set api key to do so.
else
        echo deploy to bintreay.
        $DOCKER_EXEC "cd /.build/conan && conan export conan-cpp/latest"
        $DOCKER_EXEC "conan install LibAvCpp/$TAG@conan-cpp/latest --build=missing"
        $DOCKER_EXEC "conan user -p $1 -r conan-cpp squawkcpp"
        $DOCKER_EXEC "conan upload LibAvCpp/$TAG@conan-cpp/latest --all -r=conan-cpp"
fi

