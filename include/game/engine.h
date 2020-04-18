//
// Created by alexm on 4/15/2020.
//

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H

#include <game/location.h>
#include <game/frame.h>
#include <game/player-square.h>
#include <vector>
#include <fstream>

namespace game {

class Engine {
 public:
  Engine(size_t width, size_t height);
  void Reset();
  void Step();
  PlayerSquare GetPlayerSquare() const;
  Frame GetFrame() const;
  bool PlayerIsOnGround(const std::vector<Location>& square_locs);
  bool PlayerCanMoveForward(const std::vector<Location>& square_locs);
  void AttemptJump();
  bool IsDead() const;
  bool EndReached() const;

 private:
  bool IsSquareToDirection(const std::vector<Location>& square_locs, Location direction);

 private:
  const Location kBelow = Location(0, 1);
  const Location kRight = Location(1, 0);
  const Location kStartLocation = Location(5, 5);
  const size_t width_;   // the width by number of tiles
  const size_t height_;  // the height by number of tiles
  Frame frame_;  // the frame that the game will be played in
  PlayerSquare playerSquare;
  bool attempt_jump_ = false;
  bool dead_ = false;
};

}

#endif  // FINALPROJECT_ENGINE_H
