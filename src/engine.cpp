//
// Created by alexm on 4/15/2020.
//

#include <game/engine.h>
#include <game/location.h>

namespace game {

Engine::Engine() = default;

void Engine::StartLevel(size_t map_number) {
  frame_.SetFrame(maps_[map_number - 1]);
  Reset();
}

void Engine::Step() {
  std::vector<Location> square_locs = frame_.GetSquaresNearPlayerCol();
  std::vector<Location> spike_locs = frame_.GetSpikesNearPlayerCol();
  bool square_below = ItemBelowPlayer(square_locs);
  bool spike_below = ItemBelowPlayer(spike_locs);

  if (!square_below && !spike_below) {
    if (playerSquare.TurnsRising() == 0) {
      playerSquare.Fall();
    } else {
      playerSquare.Rise();

      if (playerSquare.TurnsRising() >= 3)
      playerSquare.StopRising();
    }
  } else if (attempt_jump_) {
    playerSquare.Rise();
  }

  if (!ItemInFrontOfPlayer(square_locs) &&
      !ItemInFrontOfPlayer(spike_locs) &&
      !spike_below) {
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

bool Engine::ItemBelowPlayer(const std::vector<Location>& square_locs) {
  return IsSquareToDirection(square_locs, kBelow);
}

bool Engine::ItemInFrontOfPlayer(const std::vector<Location>& square_locs) {
  return IsSquareToDirection(square_locs, kRight);
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

void Engine::DontJump() {
  attempt_jump_ = false;
}

bool Engine::IsDead() const {
  return dead_;
}

bool Engine::EndReached() const {
  return frame_.EndReached();
}
}
