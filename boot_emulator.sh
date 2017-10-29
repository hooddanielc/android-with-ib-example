# install HAXM for osx
# sdkmanager --install 'extras;intel;Hardware_Accelerated_Execution_Manager'
# sudo /bin/bash /opt/android-sdk/extras/intel/Hardware_Accelerated_Execution_Manager/silent_install.sh
sdkmanager --install 'emulator'
sdkmanager --install 'system-images;android-25;google_apis;x86_64'
avdmanager create avd -n test -k "system-images;android-25;google_apis;x86_64" || true
$(which emulator) -avd test
