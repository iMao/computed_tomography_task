#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <memory>
#include <vector>

#include "lines.h"

namespace math {

struct Matrix {
  Matrix() = delete;
  Matrix(int rows, int cols);
  Matrix(const std::vector<unsigned int>& cluster,
         const std::vector<tmg::Line2D>& lines);
  Matrix(const Matrix& matrix);
  Matrix(Matrix&& matrix);
  Matrix& operator=(const Matrix& matrix);
  Matrix& operator=(Matrix&& matrix);
  virtual ~Matrix();

  double& at(unsigned int y, unsigned int x);
  const double& at(unsigned int y, unsigned int x) const;

  double& operator[](unsigned int i);
  const double& operator[](unsigned int i) const;

  int rows_;
  int cols_;
  std::shared_ptr<double[]> matrix_;
};

std::ostream& operator<<(std::ostream& os, Matrix& m);

void ComposeColumnVector(const std::vector<unsigned int>& cluster,
                         const std::vector<tmg::Line2D>& lines,
                         std::vector<double>& column);

void ComposeColumnVector(const std::vector<unsigned int>& cluster,
                         const std::vector<tmg::Line2D>& lines, Matrix& column);

bool MatrixMul(const Matrix& M, const Matrix& N, Matrix& P);

double Determinant(math::Matrix& matrix);

void TestMatrixMul();

void TestDeterminant();

}  // namespace math

#endif  // MATRIX_H
