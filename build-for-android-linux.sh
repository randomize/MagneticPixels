echo "Building EMBoss game"

# env: ant, sdk to path, so ant.exe and android.exe runs
#export JAVA_HOME=C:/Program\ Files\ \(x86\)/Java/jdk1.6.0_38

cd ./proj.android/

export NDK_ROOT=/opt/android-ndk

./build_native.sh
if [ $? -ne 0 ]; then exit; fi

#android list target

#COCOLIB=${COCOS2DX_ROOT}/cocos2dx/platform/android/java/
android update project -p . -t 1 -s
#android update project -p . -t 1 --library $COCOLIB -s
#android update project -p $COCOLIB -t 1

ant debug install


