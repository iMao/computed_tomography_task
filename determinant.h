#ifndef DETERMINANT_H
#define DETERMINANT_H

#include <array>
#include <optional>

namespace math {

using M3x3 = std::array<float, 9>;
using M2x2 = std::array<float, 4>;

struct LineIntersectionPoint {
  float x;
  float y;
  unsigned int L1;
  unsigned int L2;
  unsigned int L3;
};

/**
 * @brief Determinant3x3
 * @param m - matrix 3x3
 * @return
 */
float Determinant3x3(const M3x3 &m);

/**
 * @brief Determinant2x2
 * @param m
 * @return
 */
float Determinant2x2(const M2x2 &m);

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
