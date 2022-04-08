#include "lines.h"

namespace tmg {

constexpr double TOLERANCE{1.0};
constexpr double zero{0.0001};

Line2D::Line2D(unsigned int line_number, double x1, double y1, double x2,
               double y2)
    : line_number_(line_number), x1_(x1), y1_(y1), x2_(x2), y2_(y2) {
  CalcDirectLineVector();
  CalcGeneralEquationCoefficients();
}

Line2D::Line2D(const Line2D &line)
    : line_number_(line.line_number_),
      x1_(line.x1_),
      y1_(line.y1_),
      x2_(line.x2_),
      y2_(line.y2_),
      l_(line.l_),
      m_(line.m_),
      A_(line.A_),
      B_(line.B_),
      C_(line.C_) {}

Line2D &Line2D::operator=(const Line2D &line) {
  line_number_ = line.line_number_;
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

unsigned int Line2D::GetLineNumber() const { return line_number_; }

Line2D::~Line2D() {}

double Line2D::GetA() const { return A_; }
double Line2D::GetB() const { return B_; }
double Line2D::GetC() const { return C_; }

double Line2D::CalcY_from_X(float x) { return -(A_ * x + C_) / B_; }

double Line2D::GetX1() const { return x1_; }

double Line2D::GetY1() const { return y1_; }

double Line2D::GetX2() const { return x2_; }

double Line2D::GetY2() const { return y2_; }

bool Line2D::IsPointBelongLine(double x, double y) const {
  float res = A_ * x + B_ * y + C_;

  return ((res > -TOLERANCE) && (res < TOLERANCE));
}

bool Line2D::IsPointBelongLine(const cv::Point2d &point) const {
  return IsPointBelongLine(point.x, point.y);
}

double Line2D::CalcDistanceToLine(double x, double y) const {
  double nominator = std::abs(A_ * x + B_ * y + C_);

  return (nominator / denominator_);
}

void Line2D::CalcDirectLineVector() {
  l_ = x2_ - x1_;
  m_ = y2_ - y1_;

  if (std::abs(l_) < zero) {
    l_ = 0.0;
  }

  if (std::abs(m_) < zero) {
    m_ = 0.0;
  }
}

void Line2D::CalcGeneralEquationCoefficients() {
  if (l_ == 0 || m_ == 0) {
    if (l_ == 0 && m_ > 0) {
      A_ = m_;
      B_ = 0;
      C_ = -m_ * x1_;
    }

    if (l_ == 0 && m_ < 0) {
      A_ = -m_;
      B_ = 0;
      C_ = m_ * x1_;
    }

    if (m_ == 0 && l_ > 0) {
      A_ = 0;
      B_ = l_;
      C_ = -l_ * y1_;
    }

    if (m_ == 0 && l_ < 0) {
      A_ = 0;
      B_ = -l_;
      C_ = l_ * y1_;
    }
  } else {
    float lm = l_ * m_;

    if (lm > 0) {
      A_ = m_;
      B_ = -(l_);
      C_ = y1_ * l_ - x1_ * m_;
    } else {
      A_ = -(m_);
      B_ = l_;
      C_ = x1_ * m_ - y1_ * l_;
    }
  }

  denominator_ = std::sqrt(A_ * A_ + B_ * B_);
}

std::ostream &operator<<(std::ostream &os, Line2D &line) {
  os << "A: " << line.GetA() << " B: " << line.GetB() << " C: " << line.GetC();
  return os;
}

}  // namespace tmg
