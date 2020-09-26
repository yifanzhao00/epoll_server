#!/bin/bash

set -e

if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -rf `pwd`/build/*

cd `pwd`/build &&
    cmake .. &&
    make

cd ..

if [ ! -d /usr/include/epoll_server ]; then 
    mkdir /usr/include/epoll_server
fi

for header in `ls *.h`
do
    cp $header /usr/include/epoll_server
done

cp `pwd`/lib/libepoll_server.so /usr/lib

ldconfig