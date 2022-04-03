#ifndef TMGTYPES_H
#define TMGTYPES_H

#include <iostream>

namespace tmg {

struct Point2D {
  Point2D();
  Point2D(float _x, float _y, int _number_lines_through_point);
  Point2D(const Point2D& point);
  Point2D& operator=(const Point2D& point);
  ~Point2D();

  Point2D(Point2D&& point) = delete;
  Point2D& operator=(Point2D&& point) = delete;

  float x;
  float y;
  int number_lines_through_point;
};

std::ostream& operator<<(std::ostream& os, const Point2D& point);

}  // namespace tmg

#endif  // TMGTYPES_H
