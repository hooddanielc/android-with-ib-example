#include <iostream>
#include <brusque/app.h>

class my_app_t: public brusque::app_t {

public:

  float red_value;

  bool is_increasing;

  my_app_t(): red_value(0.0), is_increasing(true) {}

  virtual void on_create() {
    std::cout << "on_create" << std::endl;
  }

  virtual void on_start() {
    std::cout << "on_start" << std::endl;
  }

  virtual void on_pause() {
    app_t::stop_run_loop();
    std::cout << "on_pause" << std::endl;
  }

  virtual void on_resume() {
    app_t::start_run_loop();
    std::cout << "on_resume" << std::endl;
  }

  virtual void on_stop() {
    std::cout << "on_stop" << std::endl;
  }

  virtual void on_destroy() {
    std::cout << "on_destroy" << std::endl;
  }

  virtual void render() {
    if (is_increasing) {
      red_value += 0.01;
    } else {
      red_value -= 0.01;
    }
    if (red_value >= 1.0) {
      red_value = 1.0;
      is_increasing = false;
    }
    if (red_value <= 0.0) {
      red_value = 0.0;
      is_increasing = true;
    }

    glClearColor(red_value, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
  }

};

int android_main(int argc, char *argv[]) {
  return my_app_t{}.main(argc, argv);
}
