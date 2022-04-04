#ifndef UTILS_H
#define UTILS_H

#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>

#include "determinant.h"
#include "lines.h"
#include "points.h"

namespace tmg {

enum ReadingFileStatus {
  kREAD_SUCCESS = 0x01,
  kOPEN_FAILURE = 0x02,
  kREAD_FAILURE = 0x04

};

/**
 * @brief ReadFileLines
 * @param fname
 * @param lines
 * @param number_lines
 * @param number_points
 * @return
 */
ReadingFileStatus ReadFileLines(const std::string& fname,
                                std::vector<tmg::Line2D>& lines,
                                int& number_lines, int& number_points);

/**
 * @brief ReadFileLines
 * @param fname
 * @param points
 * @param number_points
 * @return
 */
ReadingFileStatus ReadFilePoints(const std::string& fname,
                                 std::vector<tmg::Point2D>& points,
                                 int& number_points);

/**
 * @brief PrintCrossPoints
 * @param cross_points
 * @param max_number_points - which you want to print
 */
void PrintCrossPoints(const std::vector<tmg::Point2D>& cross_points,
                      size_t max_number_points = 10);

}  // namespace tmg

#endif  // UTILS_H
