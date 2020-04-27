//
// Created by alexm on 4/15/2020.
//

#ifndef FINALPROJECT_LOCATION_H
#define FINALPROJECT_LOCATION_H

#include <cstddef>
#include <iostream>

namespace supersquareboy {

// Represents a location on the board.
class Location {
 public:
  Location(int x, int y);

  // Comparison operators.
  bool operator()(const Location& lhs, const Location& rhs) const;
  bool operator==(const Location& rhs) const;
  bool operator!=(const Location& rhs) const;
  bool operator<(const Location& rhs) const;
  bool operator<=(const Location& rhs) const;
  bool operator>(const Location& rhs) const;
  bool operator>=(const Location& rhs) const;

  // Vector operators.
  Location operator+(const Location& rhs) const;
  // Note: Always returns positive coordinates.
  Location operator%(const Location& rhs) const;
  Location operator-(const Location& rhs) const;
  Location& operator+=(const Location& rhs);
  Location operator-() const;

  // Accessors.
  int X() const;
  int Y() const;

 private:
  int X_;
  int Y_;
};

std::ostream& operator<<(std::ostream& os, const Location& location);

}  // namespace game

#endif  // FINALPROJECT_LOCATION_H
