//
// Created by alexm on 4/16/2020.
//

#include <game/frame.h>
#include <cinder/app/App.h>

#include <string>

namespace game {
Frame::Frame(const std::string& map_path, size_t width, size_t height) :
    kRows_{width},
    kCols_{height},
    row_frame_start_(0)
{
  std::ifstream map(cinder::app::getAssetPath(map_path));
  //for (size_t i = 0; i < width; i++) {
  while (!map.eof()) {
    std::string temp;
    std::getline(map, temp);
    FillLine(temp);

    grid_.push_back(temp);
  }
}

std::vector<Location> Frame::GetSquareLocs() {
  return GetCharLocs('O');
}

std::vector<Location> Frame::GetSpikeLocs() {
  return GetCharLocs('X');
}

void Frame::FrameStep() {
  row_frame_start_++;
}

void Frame::FillLine(std::string& line) {
  for (size_t i = 0; i < line.size(); i++) {
    if (line[i] != 'O' && line[i] != 'X') {
      line[i] = ' ';
    }
  }

  for (size_t i = line.size(); i < kRows_; i++) {
    line.push_back(' ');
  }
}

std::vector<Location> Frame::GetCharLocs(char ch) {
  std::vector<Location> square_locs;
  for (size_t row = row_frame_start_; row < kRows_ + row_frame_start_; row++) {
    for (size_t col = 0; col < kCols_; col++) {
      if (grid_[row][col] == ch) {
        square_locs.emplace_back(row - row_frame_start_, kCols_ - col);
      }
    }
  }

  return square_locs;
}

}