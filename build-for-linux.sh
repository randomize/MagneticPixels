#!/bin/bash

# Break on error
set -e

cd "$(dirname "$0")"

ls ./cocos2d > /dev/null

if [ $# -ne 0 ]; then

   if [ $1 = "clean" ]; then
      # rm bin
      echo "====================================================="
      echo "=================== CLEANUP ========================="
      echo "====================================================="
      rm ./cocos2d/linux-build -rf
      rm ./proj.linux/build -rf
   fi;

   #if [ $1 = "loggy" ]; then
      #LOG=" > /tmp/loggy.log"
   #else
      #LOG=""
   #fi
fi

let ncpu=`cat /proc/cpuinfo | grep processor -c`+1

echo "====================================================="
echo "=================== BUILGIND ========================"
echo "====================================================="
echo "==< $ncpu THREADS >=================================="


# rm -rf ../bin
# DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

export CC=clang
export CXX=clang++

##make global libs
#cd ./cocos2d
#mkdir -p linux-build
#cd linux-build
#cmake .. -DBUILD_LIBS_LUA=OFF -DBUILD_HelloCpp=OFF -DBUILD_TestCpp=OFF -DBUILD_HelloLua=OFF -DBUILD_TestLua=OFF
#make -j$ncpu
#
#echo "====================================================="
#echo "========================GAME========================="
#echo "====================================================="
#
## Now build project
#cd ../../proj.linux

cd ./proj.linux

# rm -rf bin
mkdir -p build
cd build
cmake  ../..
make -j$ncpu

cd ../..
#mv ../bin bin

echo "====================================================="
echo "====================LAUNCHING========================"
echo "====================================================="

# ./proj.linux/buid/bin/MyGame
# sh -c ./proj.linux/buid/bin/MyGame

