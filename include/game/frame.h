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

class Frame {
 public:
  Frame(const std::string& map_path, size_t rows, size_t height);
  std::vector<Location> GetSquareLocs();
  std::vector<Location> GetSpikeLocs();
  void FrameStep();

 private:
  void FillLine(std::string& line);
  std::vector<Location> GetCharLocs(char ch);

 private:
  const size_t kRows_;
  const size_t kCols_;

  std::vector<std::string> grid_;
  size_t row_frame_start_;
};
}

#endif  // FINALPROJECT_FRAME_H
