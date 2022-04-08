#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <opencv2/highgui.hpp>

#include "utils.h"

namespace tmg {

using CrossPoint = struct { int x, y; };
struct ComparePoints {
  bool operator()(const CrossPoint& l, const CrossPoint& r) const {
    if (l.x == r.x) {
      return l.y < r.y;
    } else if (l.x < r.x) {
      return true;
    }
    return false;
  }
};

// naive realization of algorithm of finding of cross points
void AlgorithmNaive(std::vector<tmg::Line2D>& lines,
                    std::vector<tmg::Point2D> cross_points,
                    const std::string& plot3d_fname);

// finding cross points with computer vision technique
void AlgorithmWeightedBresenhamLine(std::vector<tmg::Line2D>& lines,
                                    std::vector<tmg::Point2D> cross_points);

// finding cross points by simulteniosly chosen 3lines, probability approach
void AlgorithmAlgebraicSolution(std::vector<Line2D>& lines,
                                const cv::Rect2i& rect,
                                int needed_number_distinctive_points,
                                std::vector<tmg::Point2D>& cross_points);

void PrintCrossPoints(const std::map<tmg::CrossPoint, std::set<unsigned int>,
                                     tmg::ComparePoints>& distinctive_points);

}  // namespace tmg
#endif  // ALGORITHMS_H
