//
// Created by alexm on 4/16/2020.
//

#include <game/frame.h>
#include <cinder/app/App.h>

#include <string>

namespace game {
Frame::Frame() {
}

Frame::Frame(const std::string& level_path){
  std::ifstream map(cinder::app::getAssetPath(level_path));
  while (!map.eof()) {
    std::string temp;
    std::getline(map, temp);
    FillLine(temp);

    level_grid_.push_back(temp);
  }
}


void Frame::SetFrame(const std::string& level_path) {
  level_grid_.clear();
  std::ifstream map(cinder::app::getAssetPath(level_path));
  while (!map.eof()) {
    std::string temp;
    std::getline(map, temp);
    FillLine(temp);

    level_grid_.push_back(temp);
  }
}

std::vector<Location> Frame::GetSquareLocs() {
  return GetCharLocs('O');
}

std::vector<Location> Frame::GetSpikeLocs() {
  return GetCharLocs('X');
}

std::vector<Location> Frame::GetSquaresNearPlayerX() {
  return GetCharsNearPlayerX('O');
}

std::vector<Location> Frame::GetSpikesNearPlayerX() {
  return GetCharsNearPlayerX('X');
}

void Frame::FrameStep() {
  x_frame_start_++;
}

void Frame::Reset() {
  x_frame_start_ = 0;
  end_reached_ = false;
}

void Frame::FillLine(std::string& line) {
  for (size_t i = 0; i < line.size(); i++) {
    if (line[i] != 'O' && line[i] != 'X' && line[i] != 'E') {
      line[i] = ' ';
    }
  }

  for (size_t i = line.size(); i < game::kXs; i++) {
    line.push_back(' ');
  }
}

std::vector<Location> Frame::GetCharLocs(char ch) {
  std::vector<Location> square_locs;
  for (size_t x = x_frame_start_; x < std::min(level_grid_.size(), kXs + x_frame_start_); x++) {
    for (size_t y = 0; y < kX; y++) {
      if (level_grid_[x][y] == ch) {
        square_locs.emplace_back(x - x_frame_start_, kX - y);
      }
    }
  }

  return square_locs;
}

std::vector<Location> Frame::GetCharsNearPlayerX(char ch) {
  const size_t kPlayerOffset = 4;

  std::vector<Location> square_locs;
  for (size_t y = 0; y < kX; y++) {
    if (level_grid_[x_frame_start_ + kPlayerOffset][y] == ch) {
      square_locs.emplace_back(kPlayerOffset, kX - y);
    }

    if (level_grid_[x_frame_start_ + kPlayerOffset][y] == 'E') {
      end_reached_ = true;  // keep working on this******************
    }

    if (level_grid_[x_frame_start_ + kPlayerOffset + 1][y] == ch) {
      square_locs.emplace_back(kPlayerOffset + 1, kX - y);
    }
  }

  return square_locs;
}

bool Frame::EndReached() const {
  return end_reached_;
}

}