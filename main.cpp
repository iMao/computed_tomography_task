#include <iostream>
#include <opencv2/highgui.hpp>

#include "utils.h"

const int IMAGE_ROWS{2200};
const int IMAGE_COLS{2200};
const int SCALE_FACTOR{2};

int main(int arg, char* argv[]) {
  //
  std::cout << "Computed Tomography Task" << std::endl;

  if (arg < 3) {
    std::cout << "Number of Cmdparams is no enough" << std::endl;
    exit(-1);
  }

  std::cout << "Cmdparams: " << argv[1] << " " << argv[2] << std::endl;

  std::string fname_lines(argv[1]);
  std::string fname_points(argv[2]);
  std::string plot3d_fname("cross_points_plot3D.jpeg");

  std::vector<tmg::Line2D> lines;
  std::vector<tmg::Point2D> points;

  int number_lines;
  int number_points;
  int number_check_points;

  tmg::ReadFileLines(fname_lines, lines, number_lines, number_points);

  std::cout << "Number  lines: " << number_lines << std::endl;
  std::cout << "Number points: " << number_points << std::endl;

  tmg::ReadFilePoints(fname_points, points, number_check_points);

  std::cout << "Number  check points: " << number_check_points << std::endl;

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

  cv::imshow("cross points", scaled_gray_image);
  cv::imshow("lines", scaled_gray_image_with_lines);

  tmg::DrawCrossImagePlot3D(cross_image_U16, plot3d_fname);

  cv::waitKey();

  cv::destroyAllWindows();

  lines.clear();
  points.clear();

  return 0;
}
