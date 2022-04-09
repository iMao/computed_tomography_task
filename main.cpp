#include <algorithm>
#include <iostream>

#include "algorithms.h"
#include "cluster.h"
#include "utils.h"

int main(int arg, char* argv[]) {
  //
  std::cout << "Computed Tomography Task" << std::endl;

  if (arg < 3) {
    std::cout << "Number of Cmdparams is no enough" << std::endl;
    exit(-1);
  }

  std::cout << "Cmdparams: " << argv[1] << " " << argv[2] << std::endl;

  std::string fname_lines(argv[1]);
  std::string fname_points(argv[2]);

  std::vector<tmg::Line2D> lines;
  std::vector<tmg::Point2D> points;
  std::vector<tmg::Point2D> detected_points;

  std::vector<std::vector<unsigned int>> clusters;
  std::vector<std::vector<unsigned int>> joined_clusters;

  int min_cluster_size = 3;
  int number_lines;
  int number_points;
  int number_check_points;

  tmg::ReadFileLines(fname_lines, lines, number_lines, number_points);
  tmg::ReadFilePoints(fname_points, points, number_check_points);

  std::cout << "Number  lines: " << number_lines << std::endl;
  std::cout << "Number points: " << number_points << std::endl;
  std::cout << "Number  check points: " << number_check_points << std::endl;

  cltr::ClusteringLines(lines, -1000, 1000, -1000, 1000, clusters,
                        min_cluster_size, 0.5);

  cltr::JoiningClusters(clusters, number_points, joined_clusters);

  cltr::PrintMaxClusters(joined_clusters);

  lines.clear();
  points.clear();

  return 0;
}
