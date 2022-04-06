#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <opencv2/highgui.hpp>

#include "utils.h"

// naive realization of algorithm of finding of cross points
void AlgorithmNaive(std::vector<tmg::Line2D>& lines,
                    std::vector<tmg::Point2D> cross_points,
                    const std::string& plot3d_fname);

// finding cross points with computer vision technique
void AlgorithmWeightedBresenhamLine(std::vector<tmg::Line2D>& lines,
                                    std::vector<tmg::Point2D> cross_points);

// finding cross points by simulteniosly chosen 3lines, probability approach
void AlgorithmAlgebraicSolution(std::vector<tmg::Line2D>& lines,
                                unsigned int number_iterations,
                                int needed_number_distinctive_points,
                                std::vector<tmg::Point2D> &cross_points);

#endif  // ALGORITHMS_H
