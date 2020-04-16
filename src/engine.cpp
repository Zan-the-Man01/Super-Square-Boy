//
// Created by alexm on 4/15/2020.
//

#include <game/engine.h>
#include <game/location.h>

namespace game {
Engine::Engine(size_t width, size_t height) :
    width_{width},
    height_{height},
    player_loc_{4, 4}
    //map_{map} {
{
  Reset();
}

void Engine::Reset() {
  player_loc_ = kStartLocation;
}
Location Engine::GetPlayerLocation() const {
  return player_loc_;
}
}
