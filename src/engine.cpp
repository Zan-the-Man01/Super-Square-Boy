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
    playerSquare{4, 4},
    frame_{"map.txt", width, height} {
  Reset();
}

void Engine::Step() {
  if (!playerSquare.IsRising() && !PlayerIsOnGround(playerSquare.GetLocation())) {
    playerSquare.Fall();
  }


  frame_.FrameStep();
}

void Engine::Reset() {
  //player_loc_ = kStartLocation;
}

PlayerSquare Engine::GetPlayerSquare() const {
  return playerSquare;
}

Frame Engine::GetFrame() const {
  return frame_;
}

bool Engine::PlayerIsOnGround(Location player_loc) {
  Location valid_loc = Location(player_loc.Row(), player_loc.Col() + 1);
  std::vector<Location> square_locs = frame_.GetSquaresInPlayerCol();
  for (Location loc : square_locs) {
    if (loc == valid_loc) {
      return true;
    }
  }

  return false;
}

}
