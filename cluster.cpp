#include "cluster.h"

#include "algotools.h"

namespace cltr {

void InitClusterField(
    std::vector<unsigned int> cluster[FIELD_HEIGHT][FIELD_WIDTH]) {
  for (int n = 0; n < FIELD_HEIGHT; n++) {
    for (int m = 0; m < FIELD_WIDTH; m++) {
      cluster[n][m] = std::vector<unsigned int>();
    }
  }
}

void ClusteringLines(
    const std::vector<tmg::Line2D> &lines,
    std::vector<unsigned int> cluster[FIELD_HEIGHT][FIELD_WIDTH], double e) {
  //
  for (int n = 0; n < FIELD_HEIGHT; n++) {
    for (int m = 0; m < FIELD_WIDTH; m++) {
      cv::Point2i cluster_point(m, n);
      cv::Point2i field_point = tmg::CvtCoordinatesToImageCenter(
          cluster_point, FIELD_WIDTH, FIELD_HEIGHT);

      for (auto &l : lines) {
        double distance = l.CalcDistanceToLine(field_point.x, field_point.y);

        if (distance <= e) {
          cluster[n][m].push_back(l.GetLineNumber());
        }
      }
    }
  }
}

void PrintMaxClusters(
    std::vector<unsigned int> cluster[FIELD_HEIGHT][FIELD_WIDTH]) {
  cv::Point2i p;

  std::vector<std::pair<cv::Point2i, int>> cluster_params;

  for (int n = 0; n < FIELD_HEIGHT; n++) {
    for (int m = 0; m < FIELD_WIDTH; m++) {
      if (cluster[n][m].size() > 3) {
        p.x = m;
        p.y = n;
        cv::Point2i coord_in_center =
            tmg::CvtCoordinatesToImageCenter(p, FIELD_WIDTH, FIELD_HEIGHT);

        cluster_params.push_back(
            std::make_pair(coord_in_center, cluster[n][m].size()));
      }
    }
  }

  for (auto &cltr : cluster_params) {
    std::cout << "x:" << cltr.first.y << " y:" << cltr.first.x << " "
              << cltr.second << std::endl;
  }

  cluster_params.clear();
}

}  // namespace cltr

#include "cluster.h"
