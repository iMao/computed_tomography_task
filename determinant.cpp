#include "determinant.h"

namespace math {

float Determinant3x3(M3x3 &m) {
  float det = m[0] * (m[4] * m[8] - m[5] * m[7]) -
              m[1] * (m[3] * m[8] - m[5] * m[6]) +
              m[2] * (m[3] * m[7] - m[4] * m[6]);

  return det;
}

float Determinant2x2(M2x2 &m) {
  float det = m[0] * m[3] - m[1] * m[2];

  return det;
}

}  // namespace math
