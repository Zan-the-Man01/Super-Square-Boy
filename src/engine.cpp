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
  std::vector<Location> square_locs = frame_.GetSquaresNearPlayerCol();
  bool on_ground = PlayerIsOnGround(square_locs);

  if (!on_ground) {
    attempt_jump_ = false;
    if (playerSquare.TurnsRising() == 0) {
      playerSquare.Fall();
    } else {
      playerSquare.Rise();

      if (playerSquare.TurnsRising() >= 3)
      playerSquare.StopRising();
    }
  } else if (attempt_jump_) {
    playerSquare.Rise();
    attempt_jump_ = false;
  }

  if (PlayerCanMoveForward(square_locs)) {
    frame_.FrameStep();
  } else {
    dead_ = true;
  }
}

void Engine::Reset() {
  frame_.Reset();
  playerSquare.Reset();
  attempt_jump_ = false;
  dead_ = false;
}

PlayerSquare Engine::GetPlayerSquare() const {
  return playerSquare;
}

Frame Engine::GetFrame() const {
  return frame_;
}

bool Engine::PlayerIsOnGround(const std::vector<Location>& square_locs) {
  return IsSquareToDirection(square_locs, kBelow);
}

bool Engine::PlayerCanMoveForward(const std::vector<Location>& square_locs) {
  return !IsSquareToDirection(square_locs, kRight);
}

bool Engine::IsSquareToDirection(const std::vector<Location>& square_locs, Location direction) {
  Location player_loc = playerSquare.GetLocation();
  Location valid_loc = player_loc + direction;

  for (Location loc : square_locs) {
    if (loc == valid_loc) {
      return true;
    }
  }

  return false;
}

void Engine::AttemptJump() {
  attempt_jump_ = true;
}

bool Engine::IsDead() const {
  return dead_;
}
}
