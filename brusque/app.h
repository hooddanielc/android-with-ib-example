#pragma once

#ifdef BRUSQUE_TARGET_IOS
# error "Not Implemented"
#elif  BRUSQUE_TARGET_OSX
# error "Not Implemented"
#elif  BRUSQUE_TARGET_ANDROID
  #include "platform/android/app.h"
#elif  BRUSQUE_TARGET_LINUX
# error "Not Implemented"
#elif  BRUSQUE_TARGET_WIN
# error "Not Implemented"
#else
# error "Platform Macro Missing"
#endif

namespace brusque {

class app_t {

public:

  int exec(int argc, char *argv[]);

  int main(int argc, char *argv[]);

  virtual void on_create() {}

  virtual void on_start() {}

  virtual void on_resume() {}

  virtual void on_pause() {}

  virtual void on_stop() {}

  virtual void on_destroy() {}

  virtual void render() {}

  void start_run_loop();

  void stop_run_loop();

};  // app_t

};  // brusque
