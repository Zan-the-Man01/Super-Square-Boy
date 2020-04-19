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
  Engine();
  //explicit Engine(size_t map_number);
  void StartLevel(size_t map_number);
  void Reset();
  void Step();
  PlayerSquare GetPlayerSquare() const;
  Frame GetFrame() const;
  bool ItemBelowPlayer(const std::vector<Location>& square_locs);
  bool ItemInFrontOfPlayer(const std::vector<Location>& square_locs);
  void AttemptJump();
  bool IsDead() const;
  bool EndReached() const;

 private:
  bool IsSquareToDirection(const std::vector<Location>& square_locs, Location direction);

 private:
  const std::vector<std::string> maps_ = {"level1.txt", "level2.txt"};
  const Location kBelow = Location(0, 1);
  const Location kRight = Location(1, 0);
  const Location kStartLocation = Location(5, 5);
  Frame frame_;  // the frame that the game will be played in
  PlayerSquare playerSquare;
  bool attempt_jump_ = false;
  bool dead_ = false;
};

}

#endif  // FINALPROJECT_ENGINE_H
