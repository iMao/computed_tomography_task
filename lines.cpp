#include "lines.h"

namespace tmg {

constexpr float TOLERANCE{10.0};

Line2D::Line2D(float x1, float y1, float x2, float y2)
    : x1_(x1), y1_(y1), x2_(x2), y2_(y2) {
  CalcDirectLineVector();
  CalcGeneralEquationCoefficients();
}

Line2D::Line2D(const Line2D &line)
    : x1_(line.x1_),
      y1_(line.y1_),
      x2_(line.x2_),
      y2_(line.y2_),
      l_(line.l_),
      m_(line.m_),
      A_(line.A_),
      B_(line.B_),
      C_(line.C_) {}

Line2D &Line2D::operator=(const Line2D &line) {
  x1_ = line.x1_;
  y1_ = line.y1_;
  x2_ = line.x2_;
  y2_ = line.y2_;

  l_ = line.l_;
  m_ = line.m_;

  A_ = line.A_;
  B_ = line.B_;
  C_ = line.C_;

  return *this;
}

Line2D::~Line2D() {}

float Line2D::GetA() const { return A_; }
float Line2D::GetB() const { return B_; }
float Line2D::GetC() const { return C_; }

float Line2D::CalcY_from_X(float x) { return -(A_ * x + C_) / B_; }

float Line2D::GetX1() const { return x1_; }

float Line2D::GetY1() const { return y1_; }

float Line2D::GetX2() const { return x2_; }

float Line2D::GetY2() const { return y2_; }

bool Line2D::IsPointBelongLine(const float x, const float y) const {
  float res = A_ * x + B_ * y + C_;

  return (std::abs(res) < TOLERANCE);
}

bool Line2D::IsPointBelongLine(const cv::Point2f &point) const {
  return IsPointBelongLine(point.x, point.y);
}

void Line2D::CalcDirectLineVector() {
  l_ = x2_ - x1_;
  m_ = y2_ - y1_;
}

void Line2D::CalcGeneralEquationCoefficients() {
  float lm = l_ * m_;

  if (lm > 0) {
    A_ = m_;
    B_ = -(l_);
    C_ = y1_ * l_ - x1_ * m_;
  } else {
    A_ = -m_;
    B_ = l_;
    C_ = x1_ * m_ - y1_ * l_;
  }
}

std::ostream &operator<<(std::ostream &os, Line2D &line) {
  os << "A: " << line.GetA() << " B: " << line.GetB() << " C: " << line.GetC();
  return os;
}

}  // namespace tmg
