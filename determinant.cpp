#include "determinant.h"

namespace math {

const float near_zero{1.0};

float Determinant3x3(const M3x3 &m) {
  float det = m[0] * (m[4] * m[8] - m[5] * m[7]) -
              m[1] * (m[3] * m[8] - m[5] * m[6]) +
              m[2] * (m[3] * m[7] - m[4] * m[6]);

  return det;
}

float Determinant2x2(const M2x2 &m) {
  float det = m[0] * m[3] - m[1] * m[2];

  return det;
}

std::optional<LineIntersectionPoint> CalcIntersectionPoint(const M3x3 &m,
                                                           unsigned int L1,
                                                           unsigned int L2,
                                                           unsigned int L3) {
  float det = Determinant3x3(m);

  // check intersection condition
  if (std::abs(det) > near_zero) {
    return {};
  }

  float T = m[0] * m[0] + m[3] * m[3] + m[6] * m[6];
  float G = m[0] * m[1] + m[3] * m[4] + m[6] * m[7];
  float D = m[1] * m[1] + m[4] * m[4] + m[7] * m[7];

  float Q = -(m[0] * m[2] + m[3] * m[5] + m[6] * m[8]);
  float S = -(m[1] * m[2] + m[4] * m[5] + m[7] * m[8]);

  // Cramer's rule for matrix 2x2
  M2x2 m2x2;
  m2x2[0] = T;
  m2x2[1] = G;
  m2x2[2] = G;
  m2x2[3] = D;
  float detM = Determinant2x2(m2x2);

  M2x2 mx2x2;
  mx2x2[0] = Q;
  mx2x2[1] = G;
  mx2x2[2] = S;
  mx2x2[3] = D;
  float detMx = Determinant2x2(mx2x2);

  M2x2 my2x2;
  my2x2[0] = T;
  my2x2[1] = Q;
  my2x2[2] = G;
  my2x2[3] = S;
  float detMy = Determinant2x2(my2x2);

  LineIntersectionPoint p;
  p.x = detMx / detM;
  p.y = detMy / detM;
  p.L1 = L1;
  p.L2 = L2;
  p.L3 = L3;

  return {p};
}

}  // namespace math
