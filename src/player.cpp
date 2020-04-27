//
// Created by alexm on 4/16/2020.
//

#include <game/player.h>

namespace supersquareboy {

Player::Player() :
    loc_{0, 0}{}

Player::Player(size_t x, size_t y) :
    loc_{Location(x, y)} {
}

Player::Player(Location loc) :
    loc_(loc){}

Location Player::GetLocation() const {
  return loc_;
}

void Player::StopRising() {
  turns_rising_ = 0;
}

void Player::Fall() {
  loc_ = Location(loc_.X(), loc_.Y() + 1);
}

void Player::Rise() {
  loc_ = Location(loc_.X(), loc_.Y() - 1);
  turns_rising_++;
}

size_t Player::GetTurnsRising() {
  return turns_rising_;
}

void Player::Reset() {
  loc_ = kPlayerStartLoc;
}
}