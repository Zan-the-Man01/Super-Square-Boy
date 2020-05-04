// Copyright (c) 2020 Alexander Marcozzi. All rights reserved.

#include <game/location.h>

namespace supersquareboy {

Location::Location(int x, int y) : X_(x), Y_(y) {}

bool Location::operator==(const Location& rhs) const {
  return X_ == rhs.X_ && Y_ == rhs.Y_;
}

bool Location::operator()(const Location& lhs, const Location& rhs) const {
  return lhs <= rhs;
}

bool Location::operator!=(const Location& rhs) const {
  return !(*this == rhs);
}

bool Location::operator<(const Location& rhs) const {
  return X_ < rhs.X_ || (rhs.X_ >= X_ && Y_ < rhs.Y_);
}

bool Location::operator<=(const Location& rhs) const {
  return !(rhs < *this);
}

bool Location::operator>(const Location& rhs) const {
  return rhs < *this;
}

bool Location::operator>=(const Location& rhs) const {
  return !(*this < rhs);
}

Location Location::operator+(const Location& rhs) const {
  return {X_ + rhs.X_, Y_ + rhs.Y_};
}

Location Location::operator-(const Location& rhs) const {
  return *this + (-rhs);
}

Location Location::operator-() const { return {-X_, -Y_}; }

Location& Location::operator+=(const Location& rhs) {
  *this = *this + rhs;
  return *this;
}

int mod(int a, int b) {
  int c = a % b;
  return c + (c < 0 ? b : 0);
}

Location Location::operator%(const Location& rhs) const {
  return {mod(X_, rhs.X_), mod(Y_, rhs.Y_)};
}

int Location::X() const { return X_; }

int Location::Y() const { return Y_; }

std::ostream& operator<<(std::ostream& os, const Location& location) {
  os << "{x=" << location.X() << ", y=" << location.Y() << "}";
  return os;
}

}  // namespace supersquareboy
