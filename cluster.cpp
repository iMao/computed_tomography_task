#include "cluster.h"

#include <algorithm>
#include <memory>

#include "algotools.h"

namespace cltr {

void ClusteringLines(const std::vector<tmg::Line2D> &lines, int min_x,
                     int max_x, int min_y, int max_y,
                     std::vector<std::vector<unsigned int>> &clusters,
                     int min_cluster_size, double e) {
  // field size
  int field_width = std::abs(max_x - min_x);
  int field_height = std::abs(max_y - min_y);

  using listlines = std::vector<unsigned int>;
  std::unique_ptr<listlines[]> cluster_field(
      new listlines[field_width * field_height]);

  // initialization of cluster's field
  for (int y = 0; y < field_height; y++) {
    for (int x = 0; x < field_width; x++) {
      cluster_field[y * field_width + x] = std::vector<unsigned int>();
    }
  }

  // calc distance for all lines for every point from cluster's field
  for (int y = 0; y < field_height; y++) {
    for (int x = 0; x < field_width; x++) {
      cv::Point2i cluster_point(x, y);
      cv::Point2i field_point = tmg::CvtCoordinatesToImageCenter(
          cluster_point, field_width, field_height);

      for (auto &l : lines) {
        double distance = l.CalcDistanceToLine(field_point.x, field_point.y);

        if (distance <= e) {
          cluster_field[y * field_width + x].push_back(l.GetLineNumber());
        }
      }
    }
  }

  // select  clusters with number elements >= 3
  for (int y = 0; y < field_height; y++) {
    for (int x = 0; x < field_width; x++) {
      if (cluster_field[y * field_width + x].size() >= min_cluster_size) {
        clusters.push_back(cluster_field[y * field_width + x]);
      }
    }
  }
}

void PrintMaxClusters(const std::vector<std::vector<unsigned int>> &clusters) {
  for (auto &cluster : clusters) {
    std::cout << "lines: ";
    for (auto &n : cluster) {
      std::cout << n << " ";
    }
    std::cout << std::endl;
  }
}

void JoiningClusters(std::vector<std::vector<unsigned int>> &clusters,
                     int number_points,
                     std::vector<std::vector<unsigned int>> &joined_clusters) {
  // sort vectors
  for (auto &vec : clusters) {
    std::sort(vec.begin(), vec.end());
  }

  // remove duplicates
  std::sort(clusters.begin(), clusters.end());
  clusters.erase(std::unique(clusters.begin(), clusters.end()), clusters.end());

  // union of sets
  size_t last_size = 0;
  while (clusters.size() != last_size) {
    last_size = clusters.size();
    for (size_t i = 0; i < clusters.size() - 1; i++) {
      if (std::includes(clusters[i].begin(), clusters[i].end(),
                        clusters[i + 1].begin(), clusters[i + 1].end())) {
        clusters[i + 1].clear();
      }
    }

    clusters.erase(
        std::remove_if(clusters.begin(), clusters.end(),
                       [](auto &vec) -> bool { return vec.size() == 0; }),
        clusters.end());
  }

  last_size = 0;
  while (clusters.size() != last_size) {
    last_size = clusters.size();
    for (size_t i = 0; i < clusters.size() - 1; i++) {
      if (std::includes(clusters[i + 1].begin(), clusters[i + 1].end(),
                        clusters[i].begin(), clusters[i].end())) {
        clusters[i].clear();
      }
    }

    clusters.erase(
        std::remove_if(clusters.begin(), clusters.end(),
                       [](auto &vec) -> bool { return vec.size() == 0; }),
        clusters.end());
  }

  // sort by vector's size
  std::sort(clusters.begin(), clusters.end(),
            [](std::vector<unsigned int> &vec1, std::vector<unsigned int> &vec2)
                -> bool { return (vec1.size() > vec2.size()); });

  // select number points only
  for (int n = 0; n < number_points; n++) {
    joined_clusters.push_back(clusters[n]);
  }
}

}  // namespace cltr

#include "cluster.h"
