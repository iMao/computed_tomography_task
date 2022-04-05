#include "algotools.h"

#include <mgl2/mgl.h>
#include <string.h>

#define roundf(x) floor(x + 0.5f)

namespace tmg {

void ComposeM3x3(Line2D& L1, Line2D& L2, Line2D& L3, math::M3x3& matrix) {
  matrix[0] = L1.GetA();
  matrix[1] = L1.GetB();
  matrix[2] = L1.GetC();

  matrix[3] = L2.GetA();
  matrix[4] = L2.GetB();
  matrix[5] = L2.GetC();

  matrix[6] = L3.GetA();
  matrix[7] = L3.GetB();
  matrix[8] = L3.GetC();
}

void CalcCrossPoints(cv::Mat& cross_image_U16, const cv::Mat& mask_image_U8,
                     std::vector<Line2D>& lines) {
  int rows = cross_image_U16.rows;
  int cols = cross_image_U16.cols;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      // check mask
      if (mask_image_U8.at<uint8_t>(y, x) == 0) {
        continue;
      }

      unsigned short number_lines_trough_pixel = 0;

      cv::Point2i p(x, y);
      cv::Point2i cvtp = CvtCoordinatesToImageCenter(p, cols, rows);

      for (auto& l : lines) {
        if (l.IsPointBelongLine(cvtp.x, cvtp.y)) {
          number_lines_trough_pixel++;
        }
      }
      cross_image_U16.at<uint16_t>(y, x) = number_lines_trough_pixel;
    }
  }
}

void ShowCrossImageAsPicture(const cv::Mat& cross_image_U16,
                             cv::Mat& gray_image) {
  double min_number;
  double max_number;
  cv::minMaxIdx(cross_image_U16, &min_number, &max_number);

  for (int y = 0; y < cross_image_U16.rows; y++) {
    for (int x = 0; x < cross_image_U16.cols; x++) {
      double pix =
          static_cast<double>(cross_image_U16.at<uint16_t>(y, x)) / max_number;

      gray_image.at<uint8_t>(y, x) = static_cast<uint8_t>(255.0 * pix);
    }
  }
}

cv::Point2i CvtCoordinatesToImageCenter(cv::Point2i& p, int image_width,
                                        int image_height) {
  cv::Point2i cvtp;
  cvtp.x = p.x - image_width / 2;
  cvtp.y = -p.y + image_height / 2;
  return cvtp;
}

cv::Point2i CvtCoordinatesFromImageCenter(cv::Point2i& p, int image_width,
                                          int image_height) {
  cv::Point2i cvtp;
  cvtp.x = p.x + image_width / 2;
  cvtp.y = -p.y + image_height / 2;
  return cvtp;
}

void DrawLines(cv::Mat& gray_image, std::vector<Line2D>& lines) {
  cv::Scalar line_color(255);

  for (auto& l : lines) {
    cv::Point2i p1(l.GetX1(), l.GetY1());
    cv::Point2i p2(l.GetX2(), l.GetY2());

    cv::Point2i cvtp1 =
        CvtCoordinatesFromImageCenter(p1, gray_image.cols, gray_image.rows);
    cv::Point2i cvtp2 =
        CvtCoordinatesFromImageCenter(p2, gray_image.cols, gray_image.rows);

    cv::line(gray_image, cvtp1, cvtp2, line_color, 2);
  }
}

void DrawCrossImagePlot3D(const cv::Mat& cross_image,
                          const std::string& plot3d_name) {
  int rows = cross_image.rows;
  int cols = cross_image.cols;
  mglData data(rows, cols);
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      data.a[x + y * cols] = cross_image.at<uint16_t>(y, x);
    }
  }

  mglGraph gr;
  gr.Rotate(60, 40);
  gr.Box();
  gr.Surf(data);

  gr.Cont(data, "y");
  gr.Light(true);
  gr.Axis();
  gr.WriteFrame(plot3d_name.c_str());
}

void SelectNotZeroCrossPoints(const cv::Mat& cross_image_U16,
                              std::vector<Point2D>& cross_points) {
  for (int y = 0; y < cross_image_U16.rows; y++) {
    for (int x = 0; x < cross_image_U16.cols; x++) {
      uint16_t cross_number = cross_image_U16.at<uint16_t>(y, x);
      if (cross_number != 0) {
        cv::Point2i p(x, y);
        cv::Point2i cvtp = CvtCoordinatesToImageCenter(p, cross_image_U16.cols,
                                                       cross_image_U16.rows);
        cross_points.emplace_back(cvtp.x, cvtp.y, cross_number);
      }
    }
  }
}

void WeightedBresenhamLine(cv::Mat& cross_image_U16, int x1, int y1, int x2,
                           int y2) {
  int dx = (x2 - x1 >= 0 ? 1 : -1);
  int dy = (y2 - y1 >= 0 ? 1 : -1);

  int length_x = std::abs(x2 - x1);
  int length_y = std::abs(y2 - y1);

  int length = std::max(length_x, length_y);

  if (length == 0) {
    cross_image_U16.at<uint16_t>(y1, x1)++;
  } else {
    if (length_y <= length_x) {
      // Начальные значения
      int x = x1;
      int y = y1;
      int d = -length_x;

      // Основной цикл
      length++;
      while (length--) {
        cross_image_U16.at<uint16_t>(y, x)++;

        x += dx;
        d += 2 * length_y;
        if (d > 0) {
          d -= 2 * length_x;
          y += dy;
        }
      }
    } else {
      // Начальные значения
      int x = x1;
      int y = y1;
      int d = -length_y;

      // Основной цикл
      length++;
      while (length--) {
        cross_image_U16.at<uint16_t>(y, x)++;

        y += dy;
        d += 2 * length_x;
        if (d > 0) {
          d -= 2 * length_y;
          x += dx;
        }
      }
    }
  }
}

void DrawWeightedBresenhamLines(cv::Mat& cross_image_U16,
                                std::vector<Line2D>& lines) {
  for (auto& l : lines) {
    cv::Point2i p1(l.GetX1(), l.GetY1());
    cv::Point2i p2(l.GetX2(), l.GetY2());

    cv::Point2i cvtp1 = CvtCoordinatesFromImageCenter(p1, cross_image_U16.cols,
                                                      cross_image_U16.rows);
    cv::Point2i cvtp2 = CvtCoordinatesFromImageCenter(p2, cross_image_U16.cols,
                                                      cross_image_U16.rows);

    WeightedBresenhamLine(cross_image_U16, cvtp1.x, cvtp1.y, cvtp2.x, cvtp2.y);
  }
}

void PointsLocalization(const cv::Mat& constellation_image,
                        std::vector<Point2D>& points) {}

}  // namespace tmg
