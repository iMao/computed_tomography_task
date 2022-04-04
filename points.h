#ifndef TMGTYPES_H
#define TMGTYPES_H

#include <iostream>
#include <opencv2/core.hpp>

namespace tmg {

class Point2D : public cv::Point2f {
 public:
  Point2D();
  Point2D(float x, float y, int _number_lines_through_point);
  Point2D(const Point2D& point);
  Point2D& operator=(const Point2D& point);
  ~Point2D();

  Point2D(Point2D&& point);
  Point2D& operator=(Point2D&& point);

  int number_lines_through_point;
};

std::ostream& operator<<(std::ostream& os, const Point2D& point);

}  // namespace tmg

#endif  // TMGTYPES_H
