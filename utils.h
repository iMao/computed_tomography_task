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
 * @brief ComposeM3x3
 * @param L1
 * @param L2
 * @param L3
 * @param matrix
 */
void ComposeM3x3(tmg::Line2D& L1, tmg::Line2D& L2, tmg::Line2D& L3,
                 math::M3x3& matrix);

/**
 * @brief CvtCoordinatesToImageCenter
 * @param p
 * @param image_width
 * @param image_height
 * @return
 */
cv::Point2i CvtCoordinatesToImageCenter(cv::Point2i& p, int image_width,
                                        int image_height);

cv::Point2i CvtCoordinatesFromImageCenter(cv::Point2i& p, int image_width,
                                          int image_height);

/**
 * @brief CalcCrossPoints
 * @param cross_image_U16 - every pixel is number of lines which through itself
 * @param mask_image_U8 - picture with lines
 * @param lines - collection of lines
 */
void CalcCrossPoints(cv::Mat& cross_image_U16, const cv::Mat& mask_image_U8,
                     std::vector<tmg::Line2D>& lines);

/**
 * @brief ShowCrossImageAsPicture  that You can watch as picture
 * @param cross_image
 * @param gray_image
 */
void ShowCrossImageAsPicture(const cv::Mat& cross_image, cv::Mat& gray_image);

/**
 * @brief DrawLines
 * @param gray_image
 * @param lines
 */
void DrawLines(cv::Mat& gray_image, std::vector<tmg::Line2D>& lines);

void DrawCrossImagePlot3D(const cv::Mat& cross_image,
                          const std::string& plot3d_name);

}  // namespace tmg

#endif  // UTILS_H
