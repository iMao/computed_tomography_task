#include "points.h"

#include <iomanip>

namespace tmg {

Point2D::Point2D() : cv::Point2d(0.0, 0.0), number_lines_through_point(0) {}

Point2D::Point2D(double x, double y)
    : cv::Point2d(x, y), number_lines_through_point(0) {}

Point2D::Point2D(double x, double y, int _number_lines_through_point)
    : cv::Point2d(x, y),
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
  os << std::setw(8) << point.x << "  " << std::setw(8) << point.y << "  "
     << std::setw(4) << point.number_lines_through_point;

  return os;
}

std::fstream& operator<<(std::fstream& ofs, const Point2D& point) {
  ofs << std::setw(8) << point.x << "  " << std::setw(8) << point.y << "  "
      << std::setw(5) << point.number_lines_through_point;
  return ofs;
}

}  // namespace tmg
