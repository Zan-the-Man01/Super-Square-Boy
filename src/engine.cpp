//
// Created by alexm on 4/15/2020.
//

#include <game/engine.h>
#include <game/location.h>
#include <cinder/app/AppBase.h>

namespace game {

Engine::Engine(size_t width, size_t height) :
    width_{width},
    height_{height},
    player_loc_{4, 4},
    frame_{"map.txt", width, height} {
  Reset();
}

void Engine::Step() {
  //Location d_loc = Location(player_loc_.Row() + 1, player_loc_.Col());

  //player_loc_ = d_loc;
  frame_.FrameStep();
}

void Engine::Reset() {
  player_loc_ = kStartLocation;
}

Location Engine::GetPlayerLocation() const {
  return player_loc_;
}

Frame Engine::GetFrame() const {
  return frame_;
}
}
