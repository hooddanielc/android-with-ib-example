# clean up
rm -rf bin
rm -rf obj
rm -rf lib
rm -rf ToyKey.keystore
mkdir -p bin
mkdir -p obj
mkdir -p lib
mkdir -p src/com/example/native_activity

# define ANDROID_HOME as path to android-sdk

# create the toolchain
if [ ! -d native-toolchain ]; then
$ANDROID_HOME/ndk-bundle/build/tools/make_standalone_toolchain.py \
  --arch arm64 --api 24 --install-dir native-toolchain
fi

# copy over native_app_glue if not exists
if [ ! -f native_app_glue ]; then
  cp -R $ANDROID_HOME/ndk-bundle/sources/android/native_app_glue .
fi

# create the R.java
$ANDROID_HOME/build-tools/25.0.2/aapt package \
  -f -m  -S res -J src -M AndroidManifest.xml \
  -I ${ANDROID_HOME}/platforms/android-25/android.jar

# build R.class
javac -source 1.7 -target 1.7 -d obj -classpath $ANDROID_HOME/platforms/android-25/android.jar -sourcepath src src/com/example/native_activity/R.java

# build classes.dex
$ANDROID_HOME/build-tools/25.0.2/dx --dex --output=bin/classes.dex obj

# build the good part
ib jni/libnative-activity.so

# build an unsigned apk
mkdir -p lib/arm64-v8a
cp ../out/debug/jni/libnative-activity.so lib/arm64-v8a/libnative-activity.so

$ANDROID_HOME/build-tools/25.0.2/aapt package -f -M AndroidManifest.xml -S res -I ${ANDROID_HOME}/platforms/android-25/android.jar -F bin/NativeActivity.unsigned.apk bin
$ANDROID_HOME/build-tools/25.0.2/aapt add bin/NativeActivity.unsigned.apk \
  lib/arm64-v8a/libnative-activity.so

if [ ! -f ToyKey.keystore ]; then
  keytool \
    -genkeypair -validity 1000 -dname "CN=some company,O=Android,C=JPN" -keystore ToyKey.keystore \
    -storepass 123456 -keypass 123456 -alias NativeActivityKey -sigalg MD5withRSA -keyalg RSA
fi

# sign the apk
jarsigner -keystore ToyKey.keystore -storepass 123456 -keypass 123456 \
  -tsa http://timestamp.digicert.com \
  -signedjar bin/NativeActivity.signed.apk bin/NativeActivity.unsigned.apk NativeActivityKey

# complete apk
$ANDROID_HOME/build-tools/25.0.2/zipalign -f 4 bin/NativeActivity.signed.apk bin/NativeActivity.apk

# install
HAS_PKG="`adb shell pm list packages | grep -c 'com.example.native_activity'`"
if [ "$HAS_PKG" -eq "1" ]; then
  echo "uninstall native_activity"
  adb uninstall com.example.native_activity
fi

adb install -r bin/NativeActivity.apk

echo 'success'
