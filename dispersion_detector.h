#ifndef SRC_ADAPTIVE
#define SRC_ADAPTIVE

#include <opencv2/opencv.hpp>

#include "detector_interface.h"

namespace dt {

class DispersionDetector : public ObjectDetector {
 public:
  DispersionDetector();
  DispersionDetector(const DispersionDetector&) = delete;
  DispersionDetector(DispersionDetector&&) = delete;
  virtual ~DispersionDetector() = default;

  DispersionDetector& operator=(const DispersionDetector& dt) = delete;
  DispersionDetector& operator=(DispersionDetector&& dt) = delete;

  bool DetectObjects(const cv::Mat& image, cv::Rect& roi, int object_size_param,
                     double current_relative_threshold,
                     std::vector<cv::Rect>& objects_collection) override;
  void ShowObjects() override;

 private:
  void CalcDpr();
  void CalcSigmaEpsilon();
  void AdaptiveBinaryzation(double k);
  void LocalizeObjects(cv::Mat& binary_image, std::vector<cv::Rect>& bboxs);
  void CheckROI(cv::Rect& roi, std::vector<cv::Rect>& bboxs);
  void CleanBinaryImage();

 private:
  cv::Mat original_image_;
  cv::Mat smoothed_original_image_;
  cv::Mat dvr_;
  cv::Mat dpr_;
  cv::Mat binary_image_;
  double sigma_epsilon_;
  unsigned char filter_size_;
};

}  // namespace dt

#endif
