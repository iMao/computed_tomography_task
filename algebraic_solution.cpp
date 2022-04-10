#include "algebraic_solution.h"

#include "utils.h"

namespace tmg {

void AgebraicSolution(const std::vector<Line2D> &lines,
                      const std::vector<Point2D> &points, int number_points,
                      std::vector<Point2D> &detected_points) {
  // clusterization of all lines
  int min_cluster_size = 3;

  std::vector<std::vector<unsigned int>> clusters;
  std::vector<std::vector<unsigned int>> joined_clusters;

  cltr::ClusteringLines(lines, -1000, 1000, -1000, 1000, clusters,
                        min_cluster_size, 0.5);
  cltr::JoiningClusters(clusters, number_points, joined_clusters);

  // composition of linear system equation matrices
  std::vector<math::Matrix> matrices_AB;
  std::vector<math::Matrix> columns_C;

  tmg::ComposeLinearSystems(joined_clusters, lines, matrices_AB, columns_C);

  // Least Mean square method for convertion of
  // overdetermined linear systems to square liner systems
  std::vector<math::Matrix> transposed_matrices_AB;

  for (auto &m : matrices_AB) {
    math::Matrix tm(m.cols_, m.rows_);
    if (math::CreateTransposedMatrix(m, tm)) {
      transposed_matrices_AB.push_back(tm);
    } else {
      std::cout << "Incorrect matrix size\n";
      return;
    }
  }

  // preparing of square matrices for AX = B
  std::vector<math::Matrix> square_matrices_A;
  std::vector<math::Matrix> column_matrices_B;
  std::vector<math::Matrix> column_matrices_X;

  // creation of empty matrices
  for (size_t i = 0; i < transposed_matrices_AB.size(); i++) {
    math::Matrix sqm(transposed_matrices_AB[i].rows_, matrices_AB[i].cols_);
    math::Matrix b(transposed_matrices_AB[i].rows_, 1);
    math::Matrix x(transposed_matrices_AB[i].rows_, 1);

    square_matrices_A.push_back(sqm);
    column_matrices_B.push_back(b);
    column_matrices_X.push_back(x);
  }

  // calculation of square matrix A and column matrix B
  for (size_t i = 0; i < transposed_matrices_AB.size(); i++) {
    // left part, A[]
    math::MatrixMul(transposed_matrices_AB[i], matrices_AB[i],
                    square_matrices_A[i]);

    // right part B
    math::MatrixMul(transposed_matrices_AB[i], columns_C[i],
                    column_matrices_B[i]);
  }

  // solving of square linear systems
  // Carmer's rule usage

  for (size_t j = 0; j < square_matrices_A.size(); j++) {
    if (math::CramerRuleSolver(square_matrices_A[j], column_matrices_B[j],
                               column_matrices_X[j])) {
      tmg::Point2D p(math::RoundTooLittleValue(column_matrices_X[j].at(0, 0)),
                     math::RoundTooLittleValue(column_matrices_X[j].at(1, 0)),
                     matrices_AB[j].rows_);

      detected_points.push_back(p);
    }
  }
}

}  // namespace tmg
