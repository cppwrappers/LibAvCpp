#!/bin/bash

set -e

sudo docker pull spielhuus/clang:latest
sudo docker run -itd --name build_libavcpp -v $(pwd):/repo -v $(pwd)/build:/build -v/srv/testfiles:/testfiles spielhuus/clang

sudo docker exec build_libavcpp cmake -H/repo -B/build -G Ninja -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -Dbuild_tests=ON -Dbuild_samples=OFF -Dbuild_documentation=OFF
sudo docker exec build_libavcpp cmake --build /build
sudo docker exec build_libavcpp build/test/libavcpp-test
#sudo docker exec build_libavcpp cmake --build /build --target test

sudo docker rm -f build_libavcpp
sudo rm -rf build



