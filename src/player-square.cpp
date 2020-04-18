//
// Created by alexm on 4/16/2020.
//

#include <game/player-square.h>

namespace game {

PlayerSquare::PlayerSquare() :
    loc_{0, 0}{}

PlayerSquare::PlayerSquare(size_t row, size_t col) :
    loc_{Location(row, col)} {
}

PlayerSquare::PlayerSquare(Location loc) :
    loc_(loc){}

Location PlayerSquare::GetLocation() {
  return loc_;
}

void PlayerSquare::StopRising() {
  turns_rising_ = 0;
}

void PlayerSquare::Fall() {
  loc_ = Location(loc_.Row(), loc_.Col() + 1);
}

void PlayerSquare::Rise() {
  loc_ = Location(loc_.Row(), loc_.Col() - 1);
  turns_rising_++;
}

size_t PlayerSquare::TurnsRising() {
  return turns_rising_;
}

void PlayerSquare::Reset() {
  loc_ = kPlayerStartLoc;
}
}