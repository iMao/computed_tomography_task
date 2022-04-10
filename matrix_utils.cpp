#include "matrix_utils.h"

#include <set>

namespace tmg {

void ComposeLinearSystems(
    const std::vector<std::vector<unsigned int>> &joined_clusters,
    const std::vector<Line2D> &lines, std::vector<math::Matrix> &matrices_AB,
    std::vector<math::Matrix> &columns_C) {
  //
  for (auto &cluster : joined_clusters) {
    math::Matrix ab(cluster.size(), 2);
    math::Matrix c(cluster.size(), 1);

    math::ComposeMatrixFromClusterLines(cluster, lines, ab);
    math::ComposeColumnVector(cluster, lines, c);

    matrices_AB.push_back(std::move(ab));
    columns_C.push_back(std::move(c));
  }
}

}  // namespace tmg
