#include "points.h"

#include <iomanip>

namespace tmg {

Point2D::Point2D() : cv::Point2f(0.0f, 0.0f), number_lines_through_point(0) {}

Point2D::Point2D(float x, float y, int _number_lines_through_point)
    : cv::Point2f(x, y),
      number_lines_through_point(_number_lines_through_point) {}

Point2D::Point2D(const Point2D& point) {
  x = point.x;
  y = point.y;
  number_lines_through_point = point.number_lines_through_point;
}

Point2D::Point2D(Point2D&& point) {
  x = point.x;
  y = point.y;
  number_lines_through_point = point.number_lines_through_point;
}

Point2D& Point2D::operator=(const Point2D& point) {
  x = point.x;
  y = point.y;
  number_lines_through_point = point.number_lines_through_point;
  return *this;
}

Point2D& Point2D::operator=(Point2D&& point) {
  x = point.x;
  y = point.y;
  number_lines_through_point = point.number_lines_through_point;
  return *this;
}

Point2D::~Point2D() {}

std::ostream& operator<<(std::ostream& os, const Point2D& point) {
  os << "x:" << std::setw(6) << point.x << " y:" << std::setw(6) << point.y
     << " lines:" << std::setw(4) << point.number_lines_through_point;

  return os;
}

}  // namespace tmg
