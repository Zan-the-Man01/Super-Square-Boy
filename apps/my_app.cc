// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

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
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::system_clock;
using game::Location;

using cinder::app::KeyEvent;

MyApp::MyApp() : speed_{80}, tile_size_{40}, engine_{26, 16} {}

void MyApp::setup() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();
}

void MyApp::update() {
  const auto time = system_clock::now();

  if (time - last_time_ > std::chrono::milliseconds(speed_)) {
    engine_.Step();
    last_time_ = time;
  }
}

void MyApp::draw() {
  cinder::gl::clear();
  DrawBackground();
  DrawPlayer();
  DrawSquares();
}

void MyApp::DrawBackground() const {
  cinder::gl::clear(Color(1, 1, 1));
}

void MyApp::DrawPlayer() const {
  cinder::gl::color(Color::black());
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

void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP:
    case KeyEvent::KEY_SPACE: {
      //engine_.SetDirection(Direction::kLeft);
      //engine_.AttemptJump(); // implement this function ******************
      // make falling work ***********************************************
      // implement step function *****************************************
      break;
    }
  }
}

}  // namespace myapp
