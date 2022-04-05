#include "algorithms.h"

#include <memory>

#include "algotools.h"
#include "dispersion_detector.h"

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
