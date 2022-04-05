#ifndef DETECTOR_INTERFACE
#define DETECTOR_INTERFACE

#include <opencv2/core.hpp>

namespace dt {

class ObjectDetector {
 public:
  ObjectDetector() = default;
  ObjectDetector(const ObjectDetector&) = default;
  ObjectDetector(ObjectDetector&&) noexcept = default;
  ObjectDetector& operator=(const ObjectDetector&) = default;
  ObjectDetector& operator=(ObjectDetector&&) noexcept = default;
  virtual ~ObjectDetector() = default;

  virtual bool DetectObjects(const cv::Mat& image, cv::Rect& roi,
                             int object_size_param,
                             double current_relative_threshold,
                             std::vector<cv::Rect>& objects_collection) = 0;
  virtual void ShowObjects() = 0;
};
}  // namespace dt
#endif
