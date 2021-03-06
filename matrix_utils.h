#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include "matrix.h"

namespace tmg {

/**
 * @brief ComposeLinearSystems
 * @param joined_clusters
 * @param lines
 * @param matrices_AB
 * @param columns_C
 */
void ComposeLinearSystems(
    const std::vector<std::vector<unsigned int>> &joined_clusters,
    const std::vector<tmg::Line2D> &lines,
    std::vector<math::Matrix> &matrices_AB,
    std::vector<math::Matrix> &columns_C);

}  // namespace tmg

#endif  // MATRIX_UTILS_H
