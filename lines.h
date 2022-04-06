#ifndef LINES_H
#define LINES_H

#include <iostream>
#include <opencv2/imgproc.hpp>

#include "points.h"

namespace tmg {

class Line2D {
 public:
  Line2D() = delete;
  Line2D(unsigned int line_number, float x1, float y1, float x2, float y2);
  Line2D(const Line2D& line);
  Line2D& operator=(const Line2D& line);
  ~Line2D();

  Line2D(Line2D&& line) = default;
  Line2D& operator=(Line2D&& line) = default;

  unsigned int GetLineNumber() const;
  float GetA() const;
  float GetB() const;
  float GetC() const;

  float CalcY_from_X(float x);

  float GetX1() const;
  float GetY1() const;
  float GetX2() const;
  float GetY2() const;

  bool IsPointBelongLine(const float x, const float y) const;
  bool IsPointBelongLine(const cv::Point2f& point) const;

 private:
  void CalcDirectLineVector();
  void CalcGeneralEquationCoefficients();

 private:
  unsigned int line_number_;

  float x1_;
  float y1_;
  float x2_;
  float y2_;

  float l_;
  float m_;

  float A_;
  float B_;
  float C_;
};

std::ostream& operator<<(std::ostream& os, Line2D& line);

}  // namespace tmg

#endif  // LINES_H
