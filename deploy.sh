. ./build_vars.sh

# uninstall
HAS_PKG="`adb shell pm list packages | grep -c 'com.example.native_activity'`"
if [ "$HAS_PKG" -eq "1" ]; then
  echo "uninstall native_activity"
  adb uninstall com.example.native_activity
fi

# install
adb install -r ${IB_OUTPUT}/bin/NativeActivity.apk

# launch
adb shell am start -n com.example.native_activity/android.app.NativeActivity

# log output
adb logcat native-activity:V threaded_app:V *:E *:S
