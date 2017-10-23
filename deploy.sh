IB_CONFIG=debug
IB_TARGET=jni/libnative-activity
IB_OUTPUT=$(readlink -f ../out/${IB_CONFIG}/${IB_TARGET})

# uninstall
HAS_PKG="`adb shell pm list packages | grep -c 'com.example.native_activity'`"
if [ "$HAS_PKG" -eq "1" ]; then
  echo "uninstall native_activity"
  adb uninstall com.example.native_activity
fi

# install
adb install -r ${IB_OUTPUT}/bin/NativeActivity.apk
