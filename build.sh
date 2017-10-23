set -e
CURRENT_DIR=$(pwd)
IB_CONFIG=debug
IB_TARGET=jni/libnative-activity
mkdir -p ../out/${IB_CONFIG}/${IB_TARGET}
IB_OUTPUT=../out/${IB_CONFIG}/${IB_TARGET}
IB_OUTDIR=$(dirname $IB_OUTPUT)

if [ -z ${ANDROID_HOME} ]; then
  echo '$ANDROID_HOME not set'
  exit 1
fi

if [[ ! -d $ANDROID_HOME ]]; then
  echo '$ANDROID_HOME does not exist'
  exit 1
fi

rm -rf $IB_OUTPUT
mkdir -p ${IB_OUTPUT}/bin
mkdir -p ${IB_OUTPUT}/obj
mkdir -p ${IB_OUTPUT}/lib
mkdir -p ${IB_OUTPUT}/src/com/example/native_activity

# create the toolchain
if [ ! -d ${IB_OUTDIR}/native-toolchain ]; then
  $ANDROID_HOME/ndk-bundle/build/tools/make_standalone_toolchain.py \
    --arch arm64 --api 24 --install-dir ${IB_OUTDIR}/native-toolchain
fi

# create the R.java
$ANDROID_HOME/build-tools/25.0.2/aapt package \
  -f -m  -S res -J ${IB_OUTPUT}/src -M AndroidManifest.xml \
  -I ${ANDROID_HOME}/platforms/android-25/android.jar

# build R.class
javac -source 1.7 -target 1.7 -d ${IB_OUTPUT}/obj -classpath $ANDROID_HOME/platforms/android-25/android.jar -sourcepath ${IB_OUTPUT}/src ${IB_OUTPUT}/src/com/example/native_activity/R.java

# build classes.dex
$ANDROID_HOME/build-tools/25.0.2/dx --dex --output=${IB_OUTPUT}/bin/classes.dex ${IB_OUTPUT}/obj

# build the good part
ib --cfg ${IB_CONFIG} ${IB_TARGET}.so $@

# build an unsigned apk
mkdir -p ${IB_OUTPUT}/lib/arm64-v8a
cp ../out/debug/jni/libnative-activity.so ${IB_OUTPUT}/lib/arm64-v8a/libnative-activity.so

$ANDROID_HOME/build-tools/25.0.2/aapt package -f -M AndroidManifest.xml -S res -I ${ANDROID_HOME}/platforms/android-25/android.jar -F ${IB_OUTPUT}/bin/NativeActivity.unsigned.apk ${IB_OUTPUT}/bin

# add library
cd ${IB_OUTPUT}
$ANDROID_HOME/build-tools/25.0.2/aapt add bin/NativeActivity.unsigned.apk \
  lib/arm64-v8a/libnative-activity.so
cd ${CURRENT_DIR}

if [ ! -f ${IB_OUTPUT}/ToyKey.keystore ]; then
  keytool \
    -genkeypair -validity 1000 -dname "CN=some company,O=Android,C=JPN" -keystore ${IB_OUTPUT}/ToyKey.keystore \
    -storepass 123456 -keypass 123456 -alias NativeActivityKey -sigalg MD5withRSA -keyalg RSA
fi

# sign the apk
jarsigner -keystore ${IB_OUTPUT}/ToyKey.keystore -storepass 123456 -keypass 123456 \
  -tsa http://timestamp.digicert.com \
  -signedjar ${IB_OUTPUT}/bin/NativeActivity.signed.apk ${IB_OUTPUT}/bin/NativeActivity.unsigned.apk NativeActivityKey

# complete apk
$ANDROID_HOME/build-tools/25.0.2/zipalign -f 4 ${IB_OUTPUT}/bin/NativeActivity.signed.apk ${IB_OUTPUT}/bin/NativeActivity.apk

echo 'sucessful build'
