// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <game/engine.h>

namespace myapp {

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;

 private:
  void DrawBackground() const;
  void DrawPlayer() const;
  void DrawSquares() const;

 private:
  game::Engine engine_;
  const size_t tile_size_;
  const size_t speed_;
  std::chrono::time_point<std::chrono::system_clock> last_time_;
  std::chrono::time_point<std::chrono::system_clock> time_of_death_;
  bool just_died_ = true;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
