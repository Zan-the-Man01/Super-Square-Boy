//
// Created by alexm on 4/16/2020.
//

#include <game/player-square.h>

namespace game {

PlayerSquare::PlayerSquare() :
    loc_{0, 0},
    rising_(false) {}

PlayerSquare::PlayerSquare(size_t row, size_t col) :
    loc_{Location(row, col)},
    rising_(false) {
}

PlayerSquare::PlayerSquare(Location loc) :
    loc_(loc),
    rising_(false) {}

Location PlayerSquare::GetLocation() {
  return loc_;
}

bool PlayerSquare::IsRising() {
  return rising_;
}

void PlayerSquare::Jump() {
  rising_ = true;
}

void PlayerSquare::Land() {
  rising_ = false;
}

void PlayerSquare::Fall() {
  loc_ = Location(loc_.Row(), loc_.Col() + 1);
}

}