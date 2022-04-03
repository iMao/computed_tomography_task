#include "points.h"

namespace tmg {

Point2D::Point2D() : x(0.0f), y(0.0f), number_lines_through_point(0) {}

Point2D::Point2D(float _x, float _y, int _number_lines_through_point)
    : x(_x), y(_y), number_lines_through_point(_number_lines_through_point) {}

Point2D::Point2D(const Point2D& point)
    : x(point.x),
      y(point.y),
      number_lines_through_point(point.number_lines_through_point) {}

Point2D& Point2D::operator=(const Point2D& point) {
  x = point.x;
  y = point.y;
  number_lines_through_point = point.number_lines_through_point;
  return *this;
}

Point2D::~Point2D() {}

std::ostream& operator<<(std::ostream& os, const Point2D& point) {
  os << "x:" << point.x << " y:" << point.y
     << " number lines:" << point.number_lines_through_point;

  return os;
}

}  // namespace tmg
