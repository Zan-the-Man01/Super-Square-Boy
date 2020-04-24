// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <choreograph/Choreograph.h>
#include <cinder/app/App.h>
#include <cinder/audio/audio.h>
#include <game/engine.h>

namespace myapp {

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
  void keyUp(cinder::app::KeyEvent) override;

 private:
  void DrawBackground() const;
  void DrawPlayer() const;
  void DrawSquares() const;
  void DrawSpikes() const;
  void DrawEndScreen() const;
  void DrawPauseScreen() const;
  void DrawMainMenu() const;
  cinder::Color PercentFade(cinder::Color col) const;
  bool FadeEnded() const;

 private:
  game::Engine engine_;
  const size_t tile_size_;
  const size_t speed_;
  std::chrono::time_point<std::chrono::system_clock> last_time_;
  std::chrono::time_point<std::chrono::system_clock> time_of_death_;
  std::chrono::time_point<std::chrono::system_clock> time_of_end_reached_;
  bool just_died_ = true;
  bool end_reached_ = false;
  bool paused_ = false;
  bool in_main_menu_ = true;
  std::vector<cinder::audio::VoiceRef> sound_tracks_;
  ch::Timeline timeline_;
  ch::Output<ci::vec2> _position_a;
  ch::Output<ci::vec2> _position_b;

  ch::Output<ci::vec2> _reference_bounce;
  ch::Output<ci::vec2> _reference_slide;
  double t_ = 0;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
