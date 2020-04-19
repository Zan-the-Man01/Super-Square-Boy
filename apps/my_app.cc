// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/Font.h>
#include <cinder/Text.h>
#include <cinder/Vector.h>
#include <cinder/app/App.h>
#include <cinder/audio/audio.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <game/location.h>

#include <chrono>

// change the name of this, the cinder one is already App
namespace myapp {

using cinder::Color;
using cinder::ColorA;
using cinder::Rectf;
using cinder::TextBox;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::system_clock;
using game::Location;

using cinder::app::KeyEvent;

const seconds kCountdownTime = seconds(2);
const char kNormalFont[] = "Azonix";
const char kBoldFont[] = "Arial Bold";
const char kDifferentFont[] = "Papyrus";
const std::vector<Color> player_colors = {Color(1, 0.501, 0)};
const std::vector<Color> backgr_colors = {Color(0, 0.933, 0.921)};

MyApp::MyApp() : speed_{50}, tile_size_{40}, engine_{26, 16} {}

void MyApp::setup() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();
}

template <typename C>
void PrintText(const std::string& text, const C& color, const cinder::ivec2& size,
               const cinder::vec2& loc, const size_t font_size) {
  cinder::gl::color(color);

  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font(kNormalFont, font_size))
      .size(size)
      .color(color)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}

void MyApp::update() {
  if (in_main_menu_ || paused_) {
    return;
  }

  const auto time = system_clock::now();
  if (!end_reached_ && engine_.EndReached()) {
    time_of_end_reached_ = time;
    end_reached_ = true;
  }

  if (FadeEnded()) {
    return;
  }

  if (engine_.IsDead()) {
    return;
  }


  if (time - last_time_ > std::chrono::milliseconds(speed_)) {
    engine_.Step();
    last_time_ = time;
  }
}

void MyApp::draw() {
  if (in_main_menu_) {
    DrawMainMenu();
    return;
  }
  if (paused_) {
    DrawPauseScreen();
    return;
  }
  if (FadeEnded()) {
    DrawEndScreen();
    return;
  }

  if (engine_.IsDead()) {
    const auto time = system_clock::now();
    if (just_died_) {
      just_died_ = false;
      time_of_death_ = time;
    } else if (time - time_of_death_ > std::chrono::milliseconds(1000)){
      engine_.Reset();
      just_died_ = true;
    }
  }

  cinder::gl::clear();
  DrawBackground();
  DrawPlayer();
  DrawSquares();
  DrawSpikes();
}

void MyApp::DrawBackground() const {
  cinder::gl::clear(PercentFade(backgr_colors[0]));
}

void MyApp::DrawPlayer() const {
  if (engine_.IsDead()) {
    cinder::gl::color(PercentFade(backgr_colors[0]));
  } else {
    cinder::gl::color(PercentFade(player_colors[0]));
  }

  const Location loc = engine_.GetPlayerSquare().GetLocation();
  cinder::gl::drawSolidRect(Rectf(tile_size_ * loc.Row(),
                                  tile_size_ * loc.Col(),
                                  tile_size_ * loc.Row() + tile_size_,
                                  tile_size_ * loc.Col() + tile_size_));
}

void MyApp::DrawSquares() const {
  cinder::gl::color(Color::black());
  const std::vector<Location> locs = engine_.GetFrame().GetSquareLocs();

  for (const auto& loc : locs) {
    cinder::gl::drawSolidRect(Rectf(tile_size_ * loc.Row(),
                                    tile_size_ * loc.Col(),
                                    tile_size_ * loc.Row() + tile_size_,
                                    tile_size_ * loc.Col() + tile_size_));
  }
}

void MyApp::DrawSpikes() const {
  cinder::gl::color(Color(1, 0, 0));
  const std::vector<Location> locs = engine_.GetFrame().GetSpikeLocs();

  for (const auto& loc : locs) {
    cinder::gl::drawSolidRect(Rectf(tile_size_ * loc.Row(),
                                    tile_size_ * loc.Col(),
                                    tile_size_ * loc.Row() + tile_size_,
                                    tile_size_ * loc.Col() + tile_size_));
  }
}

void MyApp::DrawEndScreen() const {
  cinder::gl::clear(Color::black());

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {1000, 100};
  const Color color = player_colors[0];


  PrintText("YOU WIN!", color, size, center, 100);
}

void MyApp::DrawPauseScreen() const {
  cinder::gl::clear(backgr_colors[0]);

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {1000, 100};
  const Color color = player_colors[0];
  const float main_y_offset = 150;
  const float small_y_offset = 75;


  PrintText("PAUSED", color, size, {center.x, center.y - main_y_offset}, 100);
  PrintText("1: RESUME GAME", color, size, center, 50);
  PrintText("2: MAIN MENU", color, size,
      {center.x, center.y + small_y_offset}, 50);
}

void MyApp::DrawMainMenu() const {
  cinder::gl::clear(backgr_colors[0]);

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {1000, 100};
  const Color color = player_colors[0];
  const float main_y_offset = 150;
  const float small_y_offset = 75;


  PrintText("SUPER SQUARE BOY", color, size, {center.x, center.y - main_y_offset}, 100);
  PrintText("1: LEVEL ONE", color, size, center, 50);
  PrintText("2: LEVEL TWO", color, size,
            {center.x, center.y + small_y_offset}, 50);
  PrintText("3: EXIT", color, size,
            {center.x, center.y + (2 * small_y_offset)}, 50);
}

void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_1: {
      if (in_main_menu_) {
        in_main_menu_ = false;
      }
      if (paused_) {
        paused_ = false;
      }
      break;
    }
    case KeyEvent::KEY_2: {
      if (in_main_menu_) {
        in_main_menu_ = false;
      }
      if (paused_) {
        in_main_menu_ = true;
        paused_ = false;
        engine_.Reset();
      }
      break;
    }
    case KeyEvent::KEY_3: {
      if (in_main_menu_) {
        exit(0);
      }
      break;
    }
    case KeyEvent::KEY_UP:
    case KeyEvent::KEY_SPACE: {
      engine_.AttemptJump();
      break;
    }
    case KeyEvent::KEY_ESCAPE: {
      if (!in_main_menu_) {
        paused_ = !paused_;
      }
      break;
    }
  }
}

Color MyApp::PercentFade(Color col) const {
  if (!engine_.EndReached()) {
    return col;
  }

  using std::chrono::milliseconds;
  const double elapsed_time =
      duration_cast<milliseconds>(system_clock::now() - time_of_end_reached_)
          .count();
  const double countdown_time = milliseconds(kCountdownTime).count();
  const double percentage = elapsed_time / countdown_time;
  return col * (1 - percentage);
}

bool MyApp::FadeEnded() const {
  Color col = PercentFade(backgr_colors[0]);
  return col.r <= 0 && col.g <= 0 && col.b <= 0;
}

}  // namespace myapp

// TODO: music

// TODO: animations with Coreograph (???)
