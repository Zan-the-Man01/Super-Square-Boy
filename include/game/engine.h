// Copyright (c) 2020 Alexander Marcozzi. All rights reserved.

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H

#include <game/frame.h>
#include <game/location.h>
#include <game/player.h>

#include <fstream>
#include <vector>

namespace supersquareboy {

/**
 * Engine that plays a level of Super Square Boy
 */
class Engine {
 public:

  /**
   * Default constructor.
   */
  Engine();

  /**
   * Loads and starts the specified level, starting from 1.
   *
   * @param level_number the number of the level to load.
   */
  void StartLevel(size_t level_number);

  /**
   * Resets the state of the engine to the start of the level.
   */
  void Reset();

  /**
   * Makes a timestep of one frame.
   */
  void Step();

  /**
   * Gets the player object.
   *
   * @return the player.
   */
  Player GetPlayerSquare() const;

  /**
   * Gets the frame object.
   *
   * @return the frame.
   */
  Frame GetFrame() const;

  /**
   * Checks if one of the items contained in the specified vector is below the
   * player.
   *
   * @param item_locs the locations of the items.
   * @return true if there is an item below the player, else false.
   */
  bool ItemBelowPlayer(const std::vector<Location>& item_locs) const;

  /**
   * Checks if one of the items contained in the specified vector is in front of
   * the player.
   *
   * @param item_locs the locations of the items.
   * @return true if there is an item in front of the player, else false.
   */
  bool ItemInFrontOfPlayer(const std::vector<Location>& item_locs) const;

  /**
   * Attempts to make the player jump.
   */
  void AttemptJump();

  /**
   * Stops attempting to jump.
   */
  void DontJump();

  /**
   * Checks if the player is dead.
   *
   * @return true if the player is dead, else false.
   */
  bool IsDead() const;

  /**
   * Checks if the player has reached the end of the level.
   *
   * @return true if the player has reached the end of the level, else false.
   */
  bool EndReached() const;

 private:

  /**
   * Checks if one of the items contained in the specified vector is in the
   * specified direction of the player.
   *
   * @param item_locs the locations of the items.
   * @param direction the direction to check.
   * @return true if there is an item in the direction, else false.
   */
  bool IsItemToDirection(const std::vector<Location>& item_locs, Location direction) const;

 private:

  /** The maximum height the player can jump. */
  const size_t kJumpHeight = 3;

  /** The names of the files containing the levels. */
  const std::vector<std::string> levels_ = {
      "level1.txt", "level2.txt", "level3.txt", "level4.txt"};

  /** Added to another location, this will give the location below. */
  const Location kBelow = Location(0, 1);

  /** Added to another location, this will give the location to the right. */
  const Location kRight = Location(1, 0);

  /** The frame that the game will be played in. */
  Frame frame_;

  /** The player. */
  Player player_square_;

  /** If the player is attempting to jump. */
  bool attempt_jump_ = false;

  /** If the player is dead. */
  bool dead_ = false;
};

}  // namespace supersquareboy

#endif  // FINALPROJECT_ENGINE_H
