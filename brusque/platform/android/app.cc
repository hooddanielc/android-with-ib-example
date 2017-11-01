#include <jni.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <android/log.h>
#include <brusque/app.h>

/* For debug builds, always enable the debug traces */
#ifndef NDEBUG
#  define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "threaded_app", __VA_ARGS__))
#  define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "threaded_app", __VA_ARGS__))
#  define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE, "threaded_app", __VA_ARGS__))
#else
#  define LOGI(...)  ((void)0)
#  define LOGE(...)  ((void)0)
#  define LOGV(...)  ((void)0)
#endif

static void onDestroy(ANativeActivity* activity) {
    LOGV("Destroy: %p\n", activity);
}

static void onStart(ANativeActivity* activity) {
    LOGV("Start: %p\n", activity);
}

static void onResume(ANativeActivity* activity) {
    LOGV("Resume: %p\n", activity);
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t *outLen) {
    void* savedState = NULL;
    LOGV("SaveInstanceState: %p\n", activity);
    // pthread_mutex_lock(&android_app->mutex);
    // android_app->stateSaved = 0;
    // android_app_write_cmd(android_app, APP_CMD_SAVE_STATE);
    // while (!android_app->stateSaved) {
    //     pthread_cond_wait(&android_app->cond, &android_app->mutex);
    // }

    // if (android_app->savedState != NULL) {
    //     savedState = android_app->savedState;
    //     *outLen = android_app->savedStateSize;
    //     android_app->savedState = NULL;
    //     android_app->savedStateSize = 0;
    // }

    // pthread_mutex_unlock(&android_app->mutex);

    return savedState;
}

static void onPause(ANativeActivity* activity) {
    LOGV("Pause: %p\n", activity);
}

static void onStop(ANativeActivity* activity) {
    LOGV("Stop: %p\n", activity);
}

static void onConfigurationChanged(ANativeActivity* activity) {
    LOGV("ConfigurationChanged: %p\n", activity);
}

static void onLowMemory(ANativeActivity* activity) {
    LOGV("LowMemory: %p\n", activity);
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused) {
    LOGV("NativeWindowFocused: %p -- %i\n", activity, focused);
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {
    LOGV("NativeWindowCreated: %p -- %p\n", activity, window);
}

static void onNativeWindowResized(ANativeActivity *activity, ANativeWindow *window) {
    LOGV("NativeWindowResized: %p -- %p\n", activity, window);
}

static void onNativeWindowRedrawNeeded(ANativeActivity *activity, ANativeWindow *window) {
    LOGV("onNativeWindowRedrawNeeded: %p -- %p\n", activity, window);
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
    LOGV("NativeWindowDestroyed: %p -- %p\n", activity, window);
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {
    LOGV("InputQueueCreated: %p -- %p\n", activity, queue);
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {
    LOGV("InputQueueDestroyed: %p -- %p\n", activity, queue);
}

static void onContentRectChanged(ANativeActivity *activity, const ARect *rect) {
    LOGV(
        "onContentRectChanged: %p -- left=%i top=%i right=%i bottom=%i\n",
        activity, rect->left, rect->top, rect->right, rect->bottom
    );
}

JNIEXPORT
void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState,
                              size_t savedStateSize) {
    LOGV("Creating: %p\n", activity);
    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
    activity->callbacks->onNativeWindowResized = onNativeWindowResized;
    activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
    activity->callbacks->onContentRectChanged = onContentRectChanged;
    // todo, do something with result?
    android_main(0, nullptr);
}

namespace brusque {

static app_t *active_app = nullptr;

int app_t::exec(int argc, char *argv[]) {
  return main(argc, argv);
}

int app_t::main(int argc, char *argv[]) {
  active_app = this;
  return 0;
}

void app_t::start_run_loop() {
}

void app_t::stop_run_loop() {
}

}
