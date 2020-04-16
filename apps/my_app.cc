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
namespace App {

using cinder::Color;
using cinder::ColorA;
using cinder::Rectf;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::system_clock;
using game::Location;

using cinder::app::KeyEvent;

Game::Game() : speed_{80}, tile_size_{20}, engine_{16, 16} {}

void Game::setup() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();
}

void Game::update() {
  const auto time = system_clock::now();

  if (time - last_time_ > std::chrono::milliseconds(speed_)) {

    last_time_ = time;
  }
}

void Game::draw() {
  cinder::gl::clear();
  DrawBackground();
  DrawPlayer();
}

void Game::DrawBackground() const {
  cinder::gl::clear(Color(1, 1, 1));
}

void Game::DrawPlayer() const {
  cinder::gl::color(Color::black());
  const Location loc = engine_.GetPlayerLocation();
  cinder::gl::drawSolidRect(Rectf(tile_size_ * loc.Row(),
                                  tile_size_ * loc.Col(),
                                  tile_size_ * loc.Row() + tile_size_,
                                  tile_size_ * loc.Col() + tile_size_));
}

void Game::keyDown(KeyEvent event) { }

}  // namespace myapp
