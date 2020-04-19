//
// Created by alexm on 4/16/2020.
//

#include <game/frame.h>
#include <cinder/app/App.h>

#include <string>

namespace game {
Frame::Frame() {
}

Frame::Frame(const std::string& map_path){
  std::ifstream map(cinder::app::getAssetPath(map_path));
  while (!map.eof()) {
    std::string temp;
    std::getline(map, temp);
    FillLine(temp);

    grid_.push_back(temp);
  }
}


void Frame::SetFrame(const std::string& map_path) {
  grid_.clear();
  std::ifstream map(cinder::app::getAssetPath(map_path));
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

std::vector<Location> Frame::GetSquaresNearPlayerCol() {
  return GetCharsNearPlayerCol('O');
}

std::vector<Location> Frame::GetSpikesNearPlayerCol() {
  return GetCharsNearPlayerCol('X');
}

void Frame::FrameStep() {
  row_frame_start_++;
}

void Frame::Reset() {
  row_frame_start_ = 0;
  end_reached_ = false;
}

void Frame::FillLine(std::string& line) {
  for (size_t i = 0; i < line.size(); i++) {
    if (line[i] != 'O' && line[i] != 'X' && line[i] != 'E') {
      line[i] = ' ';
    }
  }

  for (size_t i = line.size(); i < game::kRows; i++) {
    line.push_back(' ');
  }
}

std::vector<Location> Frame::GetCharLocs(char ch) {
  std::vector<Location> square_locs;
  for (size_t row = row_frame_start_; row < std::min(grid_.size(), kRows + row_frame_start_); row++) {
    for (size_t col = 0; col < kCols; col++) {
      if (grid_[row][col] == ch) {
        square_locs.emplace_back(row - row_frame_start_, kCols - col);
      }
    }
  }

  return square_locs;
}

std::vector<Location> Frame::GetCharsNearPlayerCol(char ch) {
  const size_t kPlayerOffset = 4;

  std::vector<Location> square_locs;
  for (size_t col = 0; col < kCols; col++) {
    if (grid_[row_frame_start_ + kPlayerOffset][col] == ch) {
      square_locs.emplace_back(kPlayerOffset, kCols - col);
    }

    if (grid_[row_frame_start_ + kPlayerOffset][col] == 'E') {
      end_reached_ = true;  // keep working on this******************
    }

    if (grid_[row_frame_start_ + kPlayerOffset + 1][col] == ch) {
      square_locs.emplace_back(kPlayerOffset + 1, kCols - col);
    }
  }

  return square_locs;
}

bool Frame::EndReached() const {
  return end_reached_;
}

}