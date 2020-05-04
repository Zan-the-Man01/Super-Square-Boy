// Copyright (c) 2020 Alexander Marcozzi. All rights reserved.

#ifndef FINALPROJECT_FRAME_H
#define FINALPROJECT_FRAME_H

#include <cinder/Stream.h>
#include <game/location.h>

#include <fstream>
#include <vector>

namespace supersquareboy {
/** The number of xs in the frame. */
const size_t kXs = 26;

/** The number of columns in the frame. */
const size_t kX = 16;

/**
 * Stores a Super Square Boy level and manipulates the frame of play.
 */
class Frame {
 public:
  /**
   * Default constructor.
   */
  Frame();

  /**
   * Constructor that directly loads a level.
   *
   * @param level_path the path of the file containing the level to load.
   */
  explicit Frame(const std::string& level_path);

  /**
   * Loads the specified level.
   *
   * @param level_path the path of the file containing the level to load.
   */
  void SetFrame(const std::string& level_path);

  /**
   * Gets the locations of the squares in the current frame.
   *
   * @return a vector containing the locations of the squares.
   */
  std::vector<Location> GetSquareLocs();

  /**
   * Gets the locations of the spikes in the current frame.
   *
   * @return a vector containing the locations of the spikes.
   */
  std::vector<Location> GetSpikeLocs();

  /**
   * Gets the locations of the squares in the same column as the player, or one
   * column in front.
   *
   * @return a vector containing the locations of the squares.
   */
  std::vector<Location> GetSquaresNearPlayerX();

  /**
   * Gets the locations of the spikes in the same column as the player, or one
   * column in front.
   *
   * @return a vector containing the locations of the spikes.
   */
  std::vector<Location> GetSpikesNearPlayerX();

  /**
   * Steps forward in the frame.
   */
  void FrameStep();

  /**
   * Resets the frame to the beginning of the level.
   */
  void Reset();

  /**
   * Checks if the end has been reached.
   *
   * @return true if the end has been reached, else false.
   */
  bool EndReached() const;

 private:

  /**
   * Fill the string to the correct length and replace non-key characters with
   * whitespace.
   *
   * @param line the line to fill.
   */
  void FillLine(std::string& line);

  /**
   * Gets the locations of the specified characters in the current frame.
   *
   * @return a vector containing the locations of the characters.
   */
  std::vector<Location> GetCharLocs(char ch);

  /**
   * Gets the locations of the specified characters in the same column as the
   * player, or one column in front.
   *
   * @param ch the character to search for.
   * @return a vector containing the locations of the characters.
   */
  std::vector<Location> GetCharsNearPlayerX(char ch);

  /** The level in grid form. */
  std::vector<std::string> level_grid_;

  /** The starting position of the current frame. */
  size_t x_frame_start_ = 0;

  /** If the end has been reached. */
  bool end_reached_ = false;
};

}  // namespace supersquareboy

#endif  // FINALPROJECT_FRAME_H
