#ifndef ALGEBRAIC_SOLUTION_H
#define ALGEBRAIC_SOLUTION_H

#include <vector>

#include "cluster.h"
#include "lines.h"
#include "matrix.h"
#include "matrix_utils.h"
#include "points.h"

namespace tmg {

void AgebraicSolution(const std::vector<tmg::Line2D>& lines,
                      const std::vector<tmg::Point2D>& points,
                      int number_points,
                      std::vector<tmg::Point2D>& detected_points);

}

#endif  // ALGEBRAIC_SOLUTION_H
