#include <algorithm>
#include <iostream>

#include "algorithms.h"
#include "utils.h"

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

  AlgorithmWeightedBresenhamLine(lines, points);

  lines.clear();
  points.clear();

  return 0;
}
