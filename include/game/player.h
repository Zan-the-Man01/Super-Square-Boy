// Copyright (c) 2020 Alexander Marcozzi. All rights reserved.

#ifndef FINALPROJECT_PLAYER_H
#define FINALPROJECT_PLAYER_H

#include <game/location.h>

#include "frame.h"

namespace supersquareboy {

/**
 * Stores information and executes commands relating to the player of Super
 * Square Boy.
 */
class Player {
 public:

  /**
   * Default constructor.
   */
  Player();

  /**
   * Constructor where the x and y positions of the player are given.
   *
   * @param x the x position of the player.
   * @param y the y position of the player.
   */
  Player(size_t x, size_t y);

  /**
   * Constructor where the location of the player is given.
   *
   * @param loc the location of the player.
   */
  Player(Location loc);

  /**
   * Gets the location of the player.
   *
   * @return the location of the player.
   */
  Location GetLocation() const;

  /**
   * Causes the player to stop rising.
   */
  void StopRising();

  /**
   * Fall down one unit, changing the player's location.
   */
  void Fall();

  /**
   * Go up one unit, changing the player's location.
   */
  void Rise();

  /**
   * Gets the number of turns the player has been rising.
   *
   * @return the number of turns the player has been rising.
   */
  size_t GetTurnsRising();

  /**
   * Resets the state of the player.
   */
  void Reset();

 private:

  /** The default starting location of the player. */
  const Location kPlayerStartLoc = Location(4, kX - 1);

  /** The current location of the player. */
  Location loc_;

  /** The number of turns the player has been rising. */
  size_t turns_rising_ = 0;
};

}  // namespace supersquareboy

#endif  // FINALPROJECT_PLAYER_H
