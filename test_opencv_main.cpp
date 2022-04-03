#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

int main(int arg, char* argv[]) {
  std::string fname(argv[1]);

  cv::Mat image = cv::imread(fname);

  cv::imshow("image", image);

  cv::waitKey();

  return 0;
}
