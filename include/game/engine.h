//
// Created by alexm on 4/15/2020.
//

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H

#include <game/location.h>
#include <vector>
#include <fstream>

namespace game {

class Engine {
 public:
  Engine(size_t width, size_t height);
  void Reset();
  Location GetPlayerLocation() const;

 private:
  const Location kStartLocation = Location(5, 5);

  const size_t width_;   // the width by number of tiles
  const size_t height_;  // the height by number of tiles
  //const std::ifstream map_;   // the map
  Location player_loc_;
  std::vector<Location> square_locs_;
  std::vector<Location> spike_locs_;
};

}

#endif  // FINALPROJECT_ENGINE_H
