#include "utils.h"

#include <string.h>

#include <fstream>

#include "algotools.h"

namespace tmg {

constexpr int BUFFER_SIZE{64};
constexpr int OFFSET{10};

ReadingFileStatus ReadFileLines(const std::string& fname,
                                std::vector<Line2D>& lines, int& number_lines,
                                int& number_points) {
  char buffer[BUFFER_SIZE]{};
  char* ptr{nullptr};
  double coordinates[4];
  unsigned int line_number = 0;

  // open file
  std::ifstream ifs(fname);

  try {
    // check file
    if (!ifs.is_open()) {
      return ReadingFileStatus::kOPEN_FAILURE;
    }

    // reading of header
    ifs.getline(buffer, BUFFER_SIZE, '\n');

    ptr = strtok(buffer, " ");
    number_lines = atoi(ptr);

    ptr = strtok(nullptr, " ");
    number_points = atoi(ptr);

    memset(buffer, '\0', BUFFER_SIZE);

    // reading line by line
    for (int i = 0; i < number_lines; i++) {
      ifs.getline(buffer, BUFFER_SIZE, '\n');

      ptr = strtok(buffer, " ");
      int j = 0;
      while (ptr) {
        coordinates[j++] = atof(ptr);
        ptr = strtok(nullptr, " ");
      }

      lines.emplace_back(line_number++, coordinates[0], coordinates[1],
                         coordinates[2], coordinates[3]);
      memset(buffer, '\0', BUFFER_SIZE);
    }

    ifs.close();

  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return ReadingFileStatus::kREAD_FAILURE;
  }

  return ReadingFileStatus::kREAD_SUCCESS;
}

ReadingFileStatus ReadFilePoints(const std::string& fname,
                                 std::vector<Point2D>& points,
                                 int& number_points) {
  char buffer[BUFFER_SIZE]{};
  char* ptr{nullptr};
  double coordinates[2];
  int number_lines_through_point{0};

  // open file
  std::ifstream ifs(fname);

  try {
    // check file
    if (!ifs.is_open()) {
      return ReadingFileStatus::kOPEN_FAILURE;
    }

    // reading of header
    ifs.getline(buffer, BUFFER_SIZE, '\n');
    number_points = atoi(buffer);

    memset(buffer, '\0', BUFFER_SIZE);

    // reading line by line
    for (int i = 0; i < number_points; i++) {
      ifs.getline(buffer, BUFFER_SIZE, '\n');

      ptr = strtok(buffer, " ");

      int j = 0;
      while (ptr) {
        if (j <= 1) {
          coordinates[j] = atof(ptr);
        } else {
          number_lines_through_point = atoi(ptr);
        }
        j++;
        ptr = strtok(nullptr, " ");
      }

      points.emplace_back(coordinates[0], coordinates[1],
                          number_lines_through_point);
      memset(buffer, '\0', BUFFER_SIZE);
    }

  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    ifs.close();
    return ReadingFileStatus::kREAD_FAILURE;
  }

  return ReadingFileStatus::kREAD_SUCCESS;
}

void PrintCrossPoints(const std::vector<Point2D>& cross_points,
                      size_t max_number_points) {
  if (max_number_points > cross_points.size()) {
    max_number_points = cross_points.size();
  }

  for (int i = 0; i < max_number_points; i++) {
    std::cout << cross_points[i] << std::endl;
  }
}

bool WriteCrossPointsToFile(const std::string& outfile,
                            const std::vector<Point2D>& cross_points) {
  std::fstream ofs(outfile, ofs.out);
  if (!ofs.is_open()) {
    std::cout << "Couldn't open file " << outfile << std::endl;
    return false;
  }

  ofs << cross_points.size() << std::endl;
  for (auto& p : cross_points) {
    ofs << p << std::endl;
  }
  ofs.close();

  return true;
}

void ShowObjects(cv::Mat& image, std::vector<cv::Rect>& object_collection) {
  for (auto& rect : object_collection) {
    cv::Rect rct(rect.x - OFFSET, rect.y - OFFSET, rect.width + OFFSET,
                 rect.height + OFFSET);
    cv::rectangle(image, rect, cv::Scalar(255));
  }
}

bool CheckTest(const tmg::Line2D& l1, const tmg::Line2D& l2,
               const tmg::Line2D& l3, double x, double y) {
  math::M3x3 ABC3x3;
  tmg::ComposeM3x3(l1, l2, l3, ABC3x3);

  auto result = math::CalcIntersectionPoint(
      ABC3x3, l1.GetLineNumber(), l2.GetLineNumber(), l3.GetLineNumber());

  if (!result.has_value()) {
    return false;
  }

  if (static_cast<int>(result.value().x) != static_cast<int>(x) &&
      static_cast<int>(result.value().y) != static_cast<int>(y)) {
    return false;
  }

  std::cout << "x:" << static_cast<int>(result.value().x) << std::endl;
  std::cout << "y:" << static_cast<int>(result.value().y) << std::endl;

  return true;
}

}  // namespace tmg
