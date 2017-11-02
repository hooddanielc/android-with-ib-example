#include <jni.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cassert>
#include <stdexcept>
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

namespace brusque {

EGLDisplay display;
EGLContext context;
EGLSurface surface;

void destroy_context() {
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}

void create_context(ANativeWindow *window) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_NONE
    };

    EGLint format;
    EGLint numConfigs;
    EGLConfig config;

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires.
     * find the best match if possible, otherwise use the very first one
     */
    eglChooseConfig(display, attribs, nullptr, 0, &numConfigs);
    auto supportedConfigs = new EGLConfig[numConfigs];
    assert(supportedConfigs);
    eglChooseConfig(display, attribs, supportedConfigs, numConfigs, &numConfigs);
    assert(numConfigs);
    auto i = 0;
    for (; i < numConfigs; i++) {
        auto& cfg = supportedConfigs[i];
        EGLint r, g, b, d;
        if (eglGetConfigAttrib(display, cfg, EGL_RED_SIZE, &r)   &&
            eglGetConfigAttrib(display, cfg, EGL_GREEN_SIZE, &g) &&
            eglGetConfigAttrib(display, cfg, EGL_BLUE_SIZE, &b)  &&
            eglGetConfigAttrib(display, cfg, EGL_DEPTH_SIZE, &d) &&
            r == 8 && g == 8 && b == 8 && d == 0 ) {

            config = supportedConfigs[i];
            break;
        }
    }
    if (i == numConfigs) {
        config = supportedConfigs[0];
    }

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    surface = eglCreateWindowSurface(display, config, window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        throw std::runtime_error("eglMakeCurrent failed");
    }

    // Check openGL on the system
    auto opengl_info = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
    for (auto name : opengl_info) {
        auto info = glGetString(name);
        LOGI("OpenGL Info: %s", info);
    }
    // if (active_app) {
    //     active_app->render();
    // }
}

app_t::app_t(int argc, char *argv[]) {}

int app_t::exec(int argc, char *argv[]) {
  return main(argc, argv);
}

int app_t::main(int argc, char *argv[]) {
  return 0;
}

void app_t::start_run_loop() {
}

void app_t::stop_run_loop() {
}

}   // brusque


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
    brusque::create_context(window);
}

static void onNativeWindowResized(ANativeActivity *activity, ANativeWindow *window) {
    LOGV("NativeWindowResized: %p -- %p\n", activity, window);
}

static void onNativeWindowRedrawNeeded(ANativeActivity *activity, ANativeWindow *window) {
    LOGV("onNativeWindowRedrawNeeded: %p -- %p\n", activity, window);
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
    LOGV("NativeWindowDestroyed: %p -- %p\n", activity, window);
    brusque::destroy_context();
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
    auto *app = brusque::new_app(0, 0);
    activity->instance = app;
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
    //android_main(0, nullptr);
}
