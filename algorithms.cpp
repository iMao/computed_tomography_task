#include "algorithms.h"

#include <algorithm>
#include <ctime>
#include <map>
#include <memory>
#include <opencv2/core/types.hpp>
#include <random>
#include <set>
#include <utility>

#include "algotools.h"
#include "determinant.h"
#include "dispersion_detector.h"

namespace tmg {

const int IMAGE_ROWS{2200};
const int IMAGE_COLS{2200};
const int SCALE_FACTOR{2};

void AlgorithmNaive(std::vector<tmg::Line2D>& lines,
                    std::vector<tmg::Point2D> cross_points,
                    const std::string& plot3d_fname) {
  // matrix for work
  cv::Mat gray_image(IMAGE_ROWS, IMAGE_COLS, CV_8U);
  cv::Mat cross_image_U16(IMAGE_ROWS, IMAGE_COLS, CV_16U);
  cv::Mat gray_image_with_lines(IMAGE_ROWS, IMAGE_COLS, CV_8U);

  // fill zero value
  gray_image.setTo(0);
  cross_image_U16.setTo(0);
  gray_image_with_lines.setTo(0);

  // scaled images
  cv::Mat scaled_gray_image(IMAGE_ROWS / SCALE_FACTOR,
                            IMAGE_COLS / SCALE_FACTOR, CV_8U);
  cv::Mat scaled_gray_image_with_lines(IMAGE_ROWS / SCALE_FACTOR,
                                       IMAGE_COLS / SCALE_FACTOR, CV_8U);

  // pipeline
  tmg::DrawLines(gray_image_with_lines, lines);  // creation mask

  tmg::CalcCrossPoints(cross_image_U16, gray_image_with_lines, lines);

  tmg::ShowCrossImageAsPicture(cross_image_U16,
                               gray_image);  // convertion for picture view

  // scaling
  cv::resize(gray_image, scaled_gray_image,
             cv::Size(IMAGE_COLS / SCALE_FACTOR, IMAGE_ROWS / SCALE_FACTOR));
  cv::resize(gray_image_with_lines, scaled_gray_image_with_lines,
             cv::Size(IMAGE_COLS / SCALE_FACTOR, IMAGE_ROWS / SCALE_FACTOR));

  // show data as images
  cv::imshow("cross points", scaled_gray_image);
  cv::imshow("lines", scaled_gray_image_with_lines);

  // draw plot3D
  tmg::DrawCrossImagePlot3D(cross_image_U16, plot3d_fname);

  // select
  tmg::SelectNotZeroCrossPoints(cross_image_U16, cross_points);

  std::sort(
      cross_points.begin(), cross_points.end(),
      [&](tmg::Point2D& cross_point1, tmg::Point2D& cross_point2) -> bool {
        return (cross_point1.number_lines_through_point >
                cross_point2.number_lines_through_point);
      });

  tmg::PrintCrossPoints(cross_points, 1000);

  cv::waitKey();

  cv::destroyAllWindows();
}

void AlgorithmWeightedBresenhamLine(std::vector<tmg::Line2D>& lines,
                                    std::vector<tmg::Point2D> cross_points) {
  std::vector<cv::Rect> star_points_collection;
  cv::Rect detector_roi(0, 0, IMAGE_COLS, IMAGE_ROWS);

  // matrix for work
  cv::Mat gray_image(IMAGE_ROWS, IMAGE_COLS, CV_8U);
  cv::Mat cross_image_U16(IMAGE_ROWS, IMAGE_COLS, CV_16U);
  cv::Mat gray_image_processing(IMAGE_ROWS, IMAGE_COLS, CV_8U);

  // fill zero value
  gray_image.setTo(0);
  cross_image_U16.setTo(0);
  gray_image_processing.setTo(0);

  // scaled images
  cv::Mat scaled_gray_image(IMAGE_ROWS / SCALE_FACTOR,
                            IMAGE_COLS / SCALE_FACTOR, CV_8U);
  cv::Mat scaled_gray_image_with_lines(IMAGE_ROWS / SCALE_FACTOR,
                                       IMAGE_COLS / SCALE_FACTOR, CV_8U);

  //
  DrawWeightedBresenhamLines(cross_image_U16, lines);

  // convertion for picture view
  tmg::ShowCrossImageAsPicture(cross_image_U16, gray_image);

  // bluring
  // cv::GaussianBlur(gray_image, gray_image, cv::Size(3, 3), 3);

  // sharping

  // creation of star points detector
  std::unique_ptr<dt::ObjectDetector> star_points_detector =
      std::make_unique<dt::DispersionDetector>();

  star_points_detector->DetectObjects(gray_image, detector_roi, 5, 0.99,
                                      star_points_collection);

  tmg::ShowObjects(gray_image, star_points_collection);

  // scaling
  cv::resize(gray_image, scaled_gray_image,
             cv::Size(IMAGE_COLS / SCALE_FACTOR, IMAGE_ROWS / SCALE_FACTOR));

  // localization of points

  cv::imshow("lines", scaled_gray_image);
  cv::waitKey();

  cv::destroyAllWindows();
}

void AlgorithmAlgebraicSolution(std::vector<tmg::Line2D>& lines,
                                const cv::Rect2i& rect,
                                int needed_number_distinctive_points,
                                std::vector<tmg::Point2D>& cross_points) {
  size_t N_lines = lines.size();

  // our results with most relible data
  std::map<tmg::CrossPoint, std::set<unsigned int>, tmg::ComparePoints>
      distinctive_points;

  math::M3x3 ABC3x3;            // matrix 3x3 which save Ai,Bi,Ci coefficients
  tmg::CrossPoint cross_point;  // save coordinates(key) of intersection point
  std::set<unsigned int> three_lines;  // save three number of intersected lines

  for (unsigned int i = 0; i < N_lines; i++) {
    for (unsigned int j = i; j < N_lines / 3; j++) {
      for (unsigned int k = j; k < N_lines / 3; k++) {
        // compose of matrix
        tmg::ComposeM3x3(lines[i], lines[j], lines[k], ABC3x3);

        // solve intersection task, if success calc coordinates
        auto result = math::CalcIntersectionPoint(
            ABC3x3, lines[i].GetLineNumber(), lines[j].GetLineNumber(),
            lines[k].GetLineNumber());

        //    std::cout << "L1:" << lines[i].GetLineNumber()
        //              << " L2:" << lines[j].GetLineNumber()
        //              << " L3:" << lines[k].GetLineNumber() <<
        //              std::endl;

        // if three lines intersect
        if (result.has_value()) {
          cross_point.x = static_cast<int>(result.value().x);
          cross_point.y = static_cast<int>(result.value().y);

          if (!CheckBoundaries(rect, cross_point.x, cross_point.y)) {
            continue;
          }

          three_lines.insert(result.value().L1);
          three_lines.insert(result.value().L2);
          three_lines.insert(result.value().L3);

          // check, may be this point already exist
          auto it = distinctive_points.find(cross_point);
          // if no
          if (it == distinctive_points.end()) {
            distinctive_points.insert(std::make_pair(cross_point, three_lines));
          } else {
            for (auto& n : three_lines) {
              it->second.insert(n);  // adds unique line number in
                                     // std::set<uint32_t>
            }
          }
          three_lines.clear();
        }
      }
    }
  }
  std::cout << "Number cross point in map:" << distinctive_points.size()
            << std::endl;
  tmg::PrintCrossPoints(distinctive_points);

  // when  all iterations finished
  // we need select points with max number of intersected lines
  // most probable this points - are our points

  //      for (int j = 0; j < needed_number_distinctive_points; j++) {
  //        int max_number_lines = 0;

  //        std::map<tmg::CrossPoint, std::set<unsigned int>>::iterator
  //        current_it; auto it = distinctive_points.begin(); size_t
  //        current_number_points = distinctive_points.size(); for (size_t k
  //        = 0; k < current_number_points; k++) {
  //          if (it->second.size() > max_number_lines) {
  //            max_number_lines = it->second.size();
  //            current_it = it;
  //          }
  //          it++;
  //        }

  //        // now we have iterator at point with max intersected lines
  //        cross_points.emplace_back(it->first.x, it->first.y,
  //        max_number_lines);

  //        // we need remove point with max intersected lines
  //        distinctive_points.erase(current_it);
  //      }
}

void PrintCrossPoints(const std::map<CrossPoint, std::set<unsigned int>,
                                     ComparePoints>& distinctive_points) {
  for (auto& point : distinctive_points) {
    std::cout << "x:" << point.first.x << "y:" << point.first.y << " lines:";
    for (auto p = point.second.begin(); p != point.second.end(); ++p) {
      std::cout << *p << "-";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

}  // namespace tmg
