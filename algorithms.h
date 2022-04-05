#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <opencv2/highgui.hpp>

#include "utils.h"

// naive realization of algorithm of finding of cross points
void AlgorithmNaive(std::vector<tmg::Line2D>& lines,
                    std::vector<tmg::Point2D> cross_points,
                    const std::string& plot3d_fname);

//
void AlgorithmWeightedBresenhamLine(std::vector<tmg::Line2D>& lines,
                                    std::vector<tmg::Point2D> cross_points);

#endif  // ALGORITHMS_H
