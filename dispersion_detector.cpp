#include "dispersion_detector.h"

namespace {

const unsigned char MIN_COLOR_INTENSITY{0};
const unsigned char MAX_COLOR_INTENSITY{255};
const int MIN_SQUARE{10};
const int MAX_SQUARE{4000};
const int FORM_RATIO{2};
const int IMAGE_BORDER{10};
const int STRUCTURED_ELEMENT_SIZE{5};

void FindDarkBoundaryBoxs(const std::vector<std::vector<cv::Point>>& contours,
                          int img_width, int img_height, int min_square,
                          int max_square, int form_ratio, int image_border,
                          std::vector<cv::Rect>& bboxs) {
  int min_x{img_width};
  int max_x{0};
  int min_y{img_height};
  int max_y{0};

  for (auto& contour : contours) {
    for (auto& pt : contour) {
      if (pt.x > max_x) max_x = pt.x;
      if (pt.y > max_y) max_y = pt.y;

      if (pt.x < min_x) min_x = pt.x;
      if (pt.y < min_y) min_y = pt.y;
    }

    bboxs.emplace_back(min_x, min_y, (max_x - min_x), (max_y - min_y));

    min_x = img_width;
    max_x = 0;
    min_y = img_height;
    max_y = 0;
  }

  //удаляем объекты площадь которых меньше порогового значения
  bboxs.erase(std::remove_if(bboxs.begin(), bboxs.end(),
                             [&](cv::Rect& rect) -> bool {
                               return (rect.width * rect.height < min_square);
                             }),
              bboxs.end());

  //удаляем объекты площадь которых больше порогового значения
  bboxs.erase(std::remove_if(bboxs.begin(), bboxs.end(),
                             [&](cv::Rect& rect) -> bool {
                               return (rect.width * rect.height > max_square);
                             }),
              bboxs.end());

  //удаляем слишком вытянутые объекты
  bboxs.erase(std::remove_if(bboxs.begin(), bboxs.end(),
                             [&](cv::Rect& rect) -> bool {
                               return (rect.width / rect.height > form_ratio) ||
                                      (rect.height / rect.width > form_ratio);
                             }),
              bboxs.end());

  //удаляем если объекты на краю изображения (для избежания краевых эффектов)
  bboxs.erase(
      std::remove_if(
          bboxs.begin(), bboxs.end(),
          [&](cv::Rect& rect) -> bool {
            return (rect.x <= image_border) || (rect.y <= image_border) ||
                   ((rect.x + rect.width) >= (img_width - image_border)) ||
                   ((rect.y + rect.height) >= (img_height - image_border));
          }),
      bboxs.end());

  //удаляем вложенные объекты
  for (auto& box : bboxs) {
    bboxs.erase(std::remove_if(bboxs.begin(), bboxs.end(),
                               [&](cv::Rect& rect) -> bool {
                                 return (rect.x > box.x) && (rect.y > box.y) &&
                                        (rect.width < box.width) &&
                                        (rect.height < box.height);
                               }),
                bboxs.end());
  }
}
}  // namespace

namespace dt {

DispersionDetector::DispersionDetector() : sigma_epsilon_(0), filter_size_(0) {}

bool DispersionDetector::DetectObjects(
    const cv::Mat& image, cv::Rect& roi, int object_size_param,
    double current_relative_threshold,
    std::vector<cv::Rect>& objects_collection) {
  //создадим контейнеры при первом запуске
  if (original_image_.empty()) {
    original_image_.create(image.rows, image.cols, CV_8U);
  }

  if (smoothed_original_image_.empty()) {
    smoothed_original_image_.create(image.rows, image.cols, CV_8U);
  }
  if (binary_image_.empty()) {
    binary_image_.create(image.rows, image.cols, CV_8U);
  }

  if (dvr_.empty()) {
    dvr_.create(image.rows, image.cols, CV_64F);
  }
  if (dpr_.empty()) {
    dpr_.create(image.rows, image.cols, CV_64F);
  }

  //если изображение одноканальное просто возьмем указатель
  //иначе конвертируем цветное в одноканальное
  if (image.channels() != 3) {
    original_image_ = image;
  } else {
    cv::cvtColor(image, original_image_, cv::COLOR_BGR2GRAY);
  }

  filter_size_ = object_size_param;

  cv::medianBlur(original_image_, smoothed_original_image_, filter_size_);

  for (int y = 0; y < dvr_.rows; y++) {
    for (int x = 0; x < dvr_.cols; x++) {
      dvr_.at<double>(y, x) = original_image_.at<unsigned char>(y, x) -
                              smoothed_original_image_.at<unsigned char>(y, x);
    }
  }

  CalcDpr();
  CalcSigmaEpsilon();
  AdaptiveBinaryzation(current_relative_threshold);
  CleanBinaryImage();
  LocalizeObjects(binary_image_, objects_collection);
  CheckROI(roi, objects_collection);

  return (!objects_collection.empty());
  // std::cout << "sigma epsilon: " << sigma_epsilon_ << std::endl;
}

void DispersionDetector::ShowObjects() {
  //  cv::imshow("original_image", original_image_);
  //  cv::imshow("smoothed_original_image", smoothed_original_image_);
  //  cv::imshow("dvr", dvr_);
  //  cv::imshow("dpr", dpr_);
  cv::imshow("binary_image", binary_image_);
}

// int DispersionDetector::GetDetectorIndex() const { return number_; }

void DispersionDetector::CalcDpr() {
  cv::Mat clone_dvr_for_core_filter = dvr_.clone();
  cv::Mat clone_for_buffer = dvr_.clone();
  cv::Mat clone_dvr_for_ext_ring_filter = dvr_.clone();

  cv::boxFilter(dvr_, clone_dvr_for_core_filter, CV_64F,
                cv::Size((filter_size_ - 2), (filter_size_ - 2)),
                cv::Point(-1, -1), false);

  cv::boxFilter(dvr_, clone_for_buffer, CV_64F,
                cv::Size(filter_size_, filter_size_), cv::Point(-1, -1), false);

  for (int y = 0; y < dpr_.rows; y++) {
    for (int x = 0; x < dpr_.cols; x++) {
      clone_dvr_for_ext_ring_filter.at<double>(y, x) =
          clone_for_buffer.at<double>(y, x) -
          clone_dvr_for_core_filter.at<double>(y, x);

      dpr_.at<double>(y, x) = clone_dvr_for_core_filter.at<double>(y, x) -
                              clone_dvr_for_ext_ring_filter.at<double>(y, x);
    }
  }
}

void DispersionDetector::CalcSigmaEpsilon() {
  double sum = 0.0;
  for (int y = 0; y < dpr_.rows; y++) {
    for (int x = 0; x < dpr_.cols; x++) {
      sum += static_cast<double>(dpr_.at<double>(y, x)) *
             static_cast<double>(dpr_.at<double>(y, x));
    }
  }
  sigma_epsilon_ = sqrt(sum / static_cast<double>(dpr_.rows * dpr_.cols));
}

void DispersionDetector::AdaptiveBinaryzation(double k) {
  for (int y = 0; y < dpr_.rows; y++) {
    for (int x = 0; x < dpr_.cols; x++) {
      if (std::abs(dpr_.at<double>(y, x)) > (sigma_epsilon_ * k)) {
        binary_image_.at<unsigned char>(y, x) = MIN_COLOR_INTENSITY;
      } else {
        binary_image_.at<unsigned char>(y, x) = MAX_COLOR_INTENSITY;
      }
    }
  }
}

void DispersionDetector::LocalizeObjects(cv::Mat& binary_image,
                                         std::vector<cv::Rect>& bboxs) {
  //поиск замкнутых контуров
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(binary_image, contours, cv::noArray(), cv::RETR_LIST,
                   cv::CHAIN_APPROX_SIMPLE);

  //находим параметры ограничивающих прямоугольников вокруг темных пятен
  FindDarkBoundaryBoxs(contours, binary_image.cols, binary_image.rows,
                       MIN_SQUARE, MAX_SQUARE, FORM_RATIO, IMAGE_BORDER, bboxs);
}

void DispersionDetector::CheckROI(cv::Rect& roi, std::vector<cv::Rect>& bboxs) {
  bboxs.erase(
      std::remove_if(bboxs.begin(), bboxs.end(),
                     [&](cv::Rect& rect) -> bool {
                       if ((rect.x < roi.x) || (rect.y < roi.y) ||
                           ((rect.x + rect.width) > (roi.x + roi.width)) ||
                           ((rect.y + rect.height) > (roi.y + roi.height))) {
                         return true;
                       }
                       return false;
                     }),
      bboxs.end());
}

void DispersionDetector::CleanBinaryImage() {
  cv::Mat structural_mat = cv::getStructuringElement(
      cv::MORPH_RECT,
      cv::Size(STRUCTURED_ELEMENT_SIZE, STRUCTURED_ELEMENT_SIZE));
  cv::dilate(binary_image_, binary_image_, structural_mat);
}

}  // namespace dt
