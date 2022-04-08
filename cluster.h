#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>

#include "lines.h"

namespace cltr {

inline constexpr int FIELD_WIDTH{1000};
inline constexpr int FIELD_HEIGHT{1000};

void InitClusterField(
    std::vector<unsigned int> cluster[FIELD_HEIGHT][FIELD_WIDTH]);

/**
 * @brief ClusteringLines
 * @param lines
 * @param cluster
 * @param e
 */
void ClusteringLines(
    const std::vector<tmg::Line2D>& lines,
    std::vector<unsigned int> cluster[FIELD_HEIGHT][FIELD_WIDTH],
    double e = 0.1);

void PrintMaxClusters(
    std::vector<unsigned int> cluster[FIELD_HEIGHT][FIELD_WIDTH]);

}  // namespace cltr

#endif  // CLUSTER_H
