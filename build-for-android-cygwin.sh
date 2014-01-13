echo "Building EMBoss game"

# env: ant, sdk to path, so ant.exe and android.exe runs
export JAVA_HOME=C:/Program\ Files\ \(x86\)/Java/jdk1.6.0_38

cd ./proj.android/

export NDK_ROOT=/cygdrive/c/android/ndk/
export COCOS2DX_ROOT=/cygdrive/c/cocos2d-x
export ANDROID_SDK_ROOT=/cygdrive/c/android/sdk
export NDK_TOOLCHAIN_VERSION=4.8

export PATH="${PATH}:$ANDROID_SDK_ROOT/tools:$ANDROID_SDK_ROOT/platform-tools"

./build_native.py

#android.bat update project -p . -t 1
#android.bat update project -p ../../../../cocos2d-x/cocos2dx/platform/android/java/ -t 1

ant debug install

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

