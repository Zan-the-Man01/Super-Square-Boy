//
// Created by alexm on 4/15/2020.
//

#include <game/engine.h>
#include <game/location.h>

namespace supersquareboy {

Engine::Engine() = default;

void Engine::StartLevel(size_t level_number) {
  frame_.SetFrame(levels_[level_number - 1]);
  Reset();
}

void Engine::Step() {
  std::vector<Location> square_locs = frame_.GetSquaresNearPlayerX();
  std::vector<Location> spike_locs = frame_.GetSpikesNearPlayerX();
  bool square_below = ItemBelowPlayer(square_locs);
  bool spike_below = ItemBelowPlayer(spike_locs);

  if (!square_below && !spike_below) {
    if (player_square_.GetTurnsRising() == 0) {
      player_square_.Fall();
    } else {
      player_square_.Rise();

      if (player_square_.GetTurnsRising() >= kJumpHeight) {
        player_square_.StopRising();
      }
    }
  } else if (attempt_jump_) {
    player_square_.Rise();
  }

  // since the player is always moving forward, if there is an item in front
  // it will result in death
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
  player_square_.Reset();
  attempt_jump_ = false;
  dead_ = false;
}

Player Engine::GetPlayerSquare() const {
  return player_square_;
}

Frame Engine::GetFrame() const {
  return frame_;
}

bool Engine::ItemBelowPlayer(const std::vector<Location>& item_locs) const {
  return IsItemToDirection(item_locs, kBelow);
}

bool Engine::ItemInFrontOfPlayer(const std::vector<Location>& item_locs) const {
  return IsItemToDirection(item_locs, kRight);
}

bool Engine::IsItemToDirection(const std::vector<Location>& item_locs,
    Location direction) const {
  Location player_loc = player_square_.GetLocation();
  Location valid_loc = player_loc + direction;

  for (Location loc : item_locs) {
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
