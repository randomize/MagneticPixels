echo "Debugging EMBoss game"


export JAVA_HOME=C:/Program\ Files\ \(x86\)/Java/jdk1.6.0_38
export NDK_ROOT=/cygdrive/c/android/ndk/
export NDK_MODULE_PATH=/cygdrive/c/cocos2d-x/:/cygdrive/c/cocos2d-x/cocos2dx/platform/third_party/android/prebuilt

export PATH=$PATH:/cygdrive/c/Program\ Files\ \(x86\)/Java/jdk1.7.0_25/bin


cd ./proj.android/


# clean logcat 
adb logcat -c

#debug
# --start -- auto launch application
# --vervose -- say everything
# --force -- to replace existing if any session
"$NDK_ROOT"/ndk-gdb --verbose --start --force --nowait

