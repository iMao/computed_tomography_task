#ifndef DETERMINANT_H
#define DETERMINANT_H

#include <array>
#include <optional>

namespace math {

using M3x3 = std::array<double, 9>;
using M2x2 = std::array<double, 4>;

struct LineIntersectionPoint {
  double x;
  double y;
  unsigned int L1;
  unsigned int L2;
  unsigned int L3;
};

/**
 * @brief Determinant3x3
 * @param m - matrix 3x3
 * @return
 */
double Determinant3x3(const M3x3 &m);

/**
 * @brief Determinant2x2
 * @param m
 * @return
 */
double Determinant2x2(const M2x2 &m);

/**
 * @brief CalcIntersectionPoint
 * @param m
 * @return
 */
std::optional<LineIntersectionPoint> CalcIntersectionPoint(const M3x3 &m,
                                                           unsigned int L1,
                                                           unsigned int L2,
                                                           unsigned int L3);

}  // namespace math

#endif  // DETERMINANT_H
