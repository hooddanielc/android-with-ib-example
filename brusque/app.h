#pragma once

#include <cassert>

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

  app_t(int argc, char *argv[]);

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

class any_factory_t {

public:

  virtual app_t *new_app(int argc, char *argv[]) const noexcept = 0;

  static const any_factory_t *get_singleton() noexcept {
    assert(singleton);
    return singleton;
  }

protected:

  any_factory_t() {
    assert(!singleton);
    singleton = this;
  }

  virtual ~any_factory_t() {
    assert(singleton == this);
    singleton = nullptr;
  }

private:

  static any_factory_t *singleton;

};

inline app_t *new_app(int argc, char *argv[]) {
  return any_factory_t::get_singleton()->new_app(argc, argv);
}

template <typename some_app_t>
class factory_t final
    : public any_factory_t {
public:

  virtual app_t *new_app(int argc, char *argv[]) const noexcept override {
    return new some_app_t(argc, argv);
  }

  static const factory_t factory;

private:

  factory_t() = default;

  virtual ~factory_t() = default;

};  // factory_t<some_app_t>

template <typename some_app_t>
const factory_t<some_app_t> factory_t<some_app_t>::factory;

};  // brusque
