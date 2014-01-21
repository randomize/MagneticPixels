#!/bin/bash
# Base building script
# Randomize, 2013
set -e

# ls ../../../cocos2d-x/cocos/2d/platform/android/java/

ls ./cocos2d

echo "Building EMBoss game"

export NDK_TOOLCHAIN_VERSION=4.8
export NDK_ROOT=~/android/ndk
export ANDROID_SDK_ROOT=~/android/sdk
# export COCOS2DX_ROOT=~/cocos2d-x

export PATH="${PATH}:$ANDROID_SDK_ROOT/tools:$ANDROID_SDK_ROOT/platform-tools"

# python -c 'import os; print( os.environ[ "ANDROID_SDK_ROOT" ])'

cd ./proj.android

python2 ./build_native.py

android update project -p ../cocos2d/cocos/2d/platform/android/java/ -t 1
android update project -p ./ -t 1 -s

ant debug
ant installd


## env: ant, sdk to path, so ant.exe and android.exe runs
##export JAVA_HOME=C:/Program\ Files\ \(x86\)/Java/jdk1.6.0_38
#
#cd ./proj.android/
#
#export NDK_ROOT=/opt/android-ndk
#
#./build_native.sh
#if [ $? -ne 0 ]; then exit; fi
#
##android list target
#
##COCOLIB=${COCOS2DX_ROOT}/cocos2dx/platform/android/java/
#android update project -p . -t 1 -s
##android update project -p . -t 1 --library $COCOLIB -s
##android update project -p $COCOLIB -t 1
#
#ant debug install
#

