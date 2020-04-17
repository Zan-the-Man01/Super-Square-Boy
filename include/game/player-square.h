//
// Created by alexm on 4/16/2020.
//

#ifndef FINALPROJECT_PLAYER_SQUARE_H
#define FINALPROJECT_PLAYER_SQUARE_H

#include <game/location.h>

namespace game {

class PlayerSquare {
 public:
  PlayerSquare();
  PlayerSquare(size_t row, size_t col);
  PlayerSquare(Location loc);
  Location GetLocation();
  bool IsRising();
  void Jump();
  void Land();
  void Fall();

 private:
  Location loc_;
  bool rising_;
};

}

#endif  // FINALPROJECT_PLAYER_SQUARE_H
