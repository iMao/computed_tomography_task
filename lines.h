#ifndef LINES_H
#define LINES_H

#include <iostream>
#include <opencv2/imgproc.hpp>

#include "points.h"

namespace tmg {

class Line2D {
 public:
  Line2D() = delete;
  Line2D(unsigned int line_number, double x1, double y1, double x2, double y2);
  Line2D(const Line2D& line);
  Line2D& operator=(const Line2D& line);
  ~Line2D();

  Line2D(Line2D&& line) = default;
  Line2D& operator=(Line2D&& line) = default;

  unsigned int GetLineNumber() const;
  double GetA() const;
  double GetB() const;
  double GetC() const;

  double CalcY_from_X(float x);

  double GetX1() const;
  double GetY1() const;
  double GetX2() const;
  double GetY2() const;

  bool IsPointBelongLine(double x, double y) const;
  bool IsPointBelongLine(const cv::Point2d& point) const;

  double CalcDistanceToLine(double x, double y) const;

 private:
  void CalcDirectLineVector();
  void CalcGeneralEquationCoefficients();

 private:
  unsigned int line_number_;

  double x1_;
  double y1_;
  double x2_;
  double y2_;

  double l_;
  double m_;

  double A_;
  double B_;
  double C_;

  double denominator_;  // sqrt(A^2 + B^2)
};

std::ostream& operator<<(std::ostream& os, Line2D& line);

}  // namespace tmg

#endif  // LINES_H
