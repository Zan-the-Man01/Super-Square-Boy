//
// Created by alexm on 4/16/2020.
//

#ifndef FINALPROJECT_PLAYER_SQUARE_H
#define FINALPROJECT_PLAYER_SQUARE_H

#include <game/location.h>

#include "frame.h"

namespace game {

class PlayerSquare {
 public:
  PlayerSquare();
  PlayerSquare(size_t x, size_t y);
  PlayerSquare(Location loc);
  Location GetLocation();
  void Jump();
  void StopRising();
  void Fall();
  void Rise();
  size_t TurnsRising();
  void Reset();

 private:
  const Location kPlayerStartLoc = Location(4, kX - 1);
  Location loc_;
  size_t turns_rising_ = 0;
};

}

#endif  // FINALPROJECT_PLAYER_SQUARE_H
