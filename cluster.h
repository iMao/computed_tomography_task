#ifndef CLUSTER_H
#define CLUSTER_H

#include <set>
#include <vector>

#include "lines.h"

namespace cltr {
/**
 * @brief ClusteringLines
 * @param lines
 * @param min_x
 * @param max_x
 * @param min_y
 * @param max_y
 * @param clusters
 * @param min_cluster_size
 * @param e
 */
void ClusteringLines(const std::vector<tmg::Line2D> &lines, int min_x,
                     int max_x, int min_y, int max_y,
                     std::vector<std::vector<unsigned int>> &clusters,
                     int min_cluster_size, double e = 1.0);
/**
 * @brief PrintMaxClusters
 * @param clusters
 */
void PrintMaxClusters(const std::vector<std::vector<unsigned int>> &clusters);

/**
 * @brief JoiningClusters
 * @param clusters
 * @param number_points
 * @param joined_clusters
 */
void JoiningClusters(std::vector<std::vector<unsigned int>> &clusters,
                     int number_points,
                     std::vector<std::vector<unsigned int>> &joined_clusters);

}  // namespace cltr

#endif  // CLUSTER_H
