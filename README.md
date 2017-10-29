build android apps with ib
==========================

## install prerequisites

Instal ib [here](https://github.com/JasonL9000/ib)

Download android sdk command line tools [here](https://developer.android.com/studio/index.html#command-tools). Copy files into a directory where you want to put all android tools. I used `/opt/android-sdk/tools`. After that you should set up your environment variables.

```
export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_151.jdk/Contents/Home
export ANDROID_HOME=/opt/android-sdk
export PATH=$PATH:$HOME/src/ib
export PATH=$PATH:/opt/android-sdk/tools
export PATH=$PATH:/opt/android-sdk/tools/bin
export PATH=$PATH:/opt/android-sdk/platform-tools
```

Install android sdk tools using sdkmanager.

```
sdkmanager --update
sdkmanager ndk-bundle
sdkmanager 'build-tools;25.0.2'
sdkmanager "platforms;android-25"
```


## building the apk

`./build.sh`

## deploying apk to your device

`./deploy.sh`
