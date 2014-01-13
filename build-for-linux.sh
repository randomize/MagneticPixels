#!/bin/bash

# Break on error
set -e

cd proj.linux

if [ $# -ne 0 ]; then

   if [ $1 = "clean" ]; then
      make clean
   fi;

   #if [ $1 = "loggy" ]; then
      #LOG=" > /tmp/loggy.log"
   #else
      #LOG=""
   #fi
fi

let ncpu=`cat /proc/cpuinfo | grep processor -c`+1

make -j$ncpu DEBUG=1 CLANG=1 V=1

cd ..

sh -c ./proj.linux/bin/debug/MagneticPixels

#if [ $? -eq 0 ]; then
   #if [ "$DISPLAY" == ':1' ]; then
      #sh -c "vglrun ./PL $LOG"
   #else
      #if [[ `hostname` == "rbook" ]]; then 
         #sh -c "LD_PRELOAD=~/Downloads/glfw-2.7.6/lib/x11/libglfw.so ./PL $LOG"
      #else
         #sh -c "./PL $LOG"
      #fi
   #fi
#fi
