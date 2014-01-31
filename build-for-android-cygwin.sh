echo "Building EMBoss game"


set -e

# env: ant, sdk to path, so ant.exe and android.exe runs
export JAVA_HOME=C:/Program\ Files\ \(x86\)/Java/jdk1.7.0_45

cd ./proj.android/

export NDK_ROOT=/cygdrive/c/android-ndk-r9c
export COCOS2DX_ROOT=../cocos2d
export ANDROID_SDK_ROOT=/cygdrive/c/adt-bundle-windows-x86_64-20131030/sdk
export NDK_TOOLCHAIN_VERSION=4.8

export PATH="${PATH}:$ANDROID_SDK_ROOT/tools"
export PATH="${PATH}:$ANDROID_SDK_ROOT/platform-tools"
export PATH="${PATH}:/cygdrive/c/apache-ant-1.9.3/bin"

adb wait-for-device

#exit

./build_native.py

android.bat update project -p . -t 1
android.bat update project -p ../cocos2d/cocos/2d/platform/android/java/ -t 1

ant.bat debug install

# android update project -p ./ --target android-18
# $ android update project -p ../../../cocos2dx/platform/android/java --target android-18
#
#    $ ant debug install
#
#If the last command results in sdk.dir missing error then do: 
#
#    $ android list target
#    $ android update project -p . -t (id from step 6)
#    $ android update project -p cocos2d-x/cocos2dx/platform/android/java/ -t (id from step 6) 

# Clear all
#      proj.android/assets/
#      proj.android/obj/

# Clear only project?

