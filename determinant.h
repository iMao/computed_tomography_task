#ifndef DETERMINANT_H
#define DETERMINANT_H

#include <array>

namespace math {

using M3x3 = std::array<float, 9>;
using M2x2 = std::array<float, 4>;

/**
 * @brief Determinant3x3
 * @param m - matrix 3x3
 * @return
 */
float Determinant3x3(M3x3 &m);

/**
 * @brief Determinant2x2
 * @param m
 * @return
 */
float Determinant2x2(M2x2 &m);

}  // namespace math

#endif  // DETERMINANT_H
