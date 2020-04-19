//
// Created by alexm on 4/16/2020.
//

#ifndef FINALPROJECT_FRAME_H
#define FINALPROJECT_FRAME_H

#include <cinder/Stream.h>
#include <game/location.h>

#include <fstream>
#include <vector>

namespace game {
const size_t kRows = 26;
const size_t kCols = 16;

class Frame {

 public:
  Frame();
  explicit Frame(const std::string& map_path);
  void SetFrame(const std::string& map_path);
  std::vector<Location> GetSquareLocs();
  std::vector<Location> GetSpikeLocs();
  std::vector<Location> GetSquaresNearPlayerCol();
  std::vector<Location> GetSpikesNearPlayerCol();
  void FrameStep();
  void Reset();
  bool EndReached() const;

 private:
  void FillLine(std::string& line);
  std::vector<Location> GetCharLocs(char ch);
  std::vector<Location> GetCharsNearPlayerCol(char ch);

 private:
  std::vector<std::string> grid_;
  size_t row_frame_start_ = 0;
  bool end_reached_ = false;
};
}

#endif  // FINALPROJECT_FRAME_H
