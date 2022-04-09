#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>

#include "lines.h"

namespace cltr {

void ClusteringLines(const std::vector<tmg::Line2D> &lines, int min_x,
                     int max_x, int min_y, int max_y,
                     std::vector<std::vector<unsigned int>> &clusters,
                     int min_cluster_size, double e = 1.0);

void PrintMaxClusters(const std::vector<std::vector<unsigned int>> &clusters);

void JoiningClusters(std::vector<std::vector<unsigned int>> &clusters,
                     int number_points,
                     std::vector<std::vector<unsigned int>> &joined_clusters);

}  // namespace cltr

#endif  // CLUSTER_H
