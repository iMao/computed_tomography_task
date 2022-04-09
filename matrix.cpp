#include "matrix.h"

#include <set>

namespace math {

Matrix::Matrix(int rows, int cols)
    : rows_(rows),
      cols_(cols),
      matrix_(std::make_unique<double[]>(rows * cols)) {}

Matrix::Matrix(const std::vector<unsigned int> &cluster,
               const std::vector<tmg::Line2D> &lines)
    : rows_(cluster.size()), cols_(2) {
  matrix_ = std::make_unique<double[]>(rows_ * cols_);

  std::set<unsigned int> set_cluster(cluster.begin(), cluster.end());

  int y = 0;
  int x = 0;
  for (auto &l : lines) {
    if (set_cluster.find(l.GetLineNumber()) != set_cluster.end()) {
      matrix_[y * cols_ + x] = l.GetA();
      x++;
      matrix_[y * cols_ + x] = l.GetB();
      y++;
    }
  }
  set_cluster.clear();
}

Matrix::Matrix(const Matrix &matrix)
    : rows_(matrix.rows_),
      cols_(matrix.cols_),
      matrix_(std::make_unique<double[]>(matrix.rows_ * matrix.cols_)) {
  for (int j = 0; j < (rows_ * cols_); j++) {
    matrix_[j] = matrix.matrix_[j];
  }
}

Matrix::Matrix(Matrix &&matrix)
    : rows_(matrix.rows_),
      cols_(matrix.cols_),
      matrix_(std::move(matrix.matrix_)) {}

Matrix &Matrix::operator=(const Matrix &matrix) {
  rows_ = matrix.rows_;
  cols_ = matrix.cols_;
  matrix_ = nullptr;
  matrix_ = std::make_unique<double[]>(matrix.rows_ * matrix.cols_);
  for (int j = 0; j < (rows_ * cols_); j++) {
    matrix_[j] = matrix.matrix_[j];
  }
  return *this;
}

Matrix &Matrix::operator=(Matrix &&matrix) {
  rows_ = matrix.rows_;
  cols_ = matrix.cols_;
  matrix_ = std::move(matrix.matrix_);
  return *this;
}

Matrix::~Matrix() {}

double &Matrix::at(unsigned int y, unsigned int x) {
  return matrix_[y * cols_ + x];
}

const double &Matrix::at(unsigned int y, unsigned int x) const {
  return matrix_[y * cols_ + x];
}

double &Matrix::operator[](unsigned int i) { return matrix_[i]; }
const double &Matrix::operator[](unsigned int i) const { return matrix_[i]; }

std::ostream &operator<<(std::ostream &os, Matrix &m) {
  os << "Matrix [" << m.rows_ << "][" << m.cols_ << "]" << std::endl;
  //
  for (int y = 0; y < m.rows_; y++) {
    for (int x = 0; x < m.cols_; x++) {
      os << m[y * m.cols_ + x] << " ";
    }
    os << std::endl;
  }
  return os;
}

void ComposeColumnVector(const std::vector<unsigned int> &cluster,
                         const std::vector<tmg::Line2D> &lines,
                         std::vector<double> &column) {
  std::set<unsigned int> set_cluster(cluster.begin(), cluster.end());

  int i = 0;
  for (auto &l : lines) {
    if (set_cluster.find(l.GetLineNumber()) != set_cluster.end()) {
      column[i++] = -(l.GetC());
    }
  }
  set_cluster.clear();
}

void ComposeColumnVector(const std::vector<unsigned int> &cluster,
                         const std::vector<tmg::Line2D> &lines,
                         Matrix &column) {
  std::set<unsigned int> set_cluster(cluster.begin(), cluster.end());

  int i = 0;
  for (auto &l : lines) {
    if (set_cluster.find(l.GetLineNumber()) != set_cluster.end()) {
      column[i++] = -(l.GetC());
    }
  }
  set_cluster.clear();
}

void MatrixMul(const Matrix &M, const Matrix &N, Matrix &P) {
  for (int i = 0; i < M.rows_; i++) {
    for (int j = 0; j < N.cols_; j++) {
      for (int k = 0; k < M.cols_; k++) {
        P.at(i, j) += M.at(i, k) * N.at(k, j);
      }
    }
  }
}

void TestMatrixMul() {
  Matrix M(2, 3);
  Matrix N(3, 2);
  Matrix P(2, 2);

  Matrix C(3, 1);
  Matrix R(2, 1);

  //------------
  M.at(0, 0) = 3;
  M.at(1, 0) = 0;

  M.at(0, 1) = 2;
  M.at(1, 1) = 2;

  M.at(0, 2) = 1;
  M.at(1, 2) = 4;

  //------------

  N.at(0, 0) = 2;
  N.at(1, 0) = 1;
  N.at(2, 0) = 3;

  N.at(0, 1) = 1;
  N.at(1, 1) = 2;
  N.at(2, 1) = 2;

  //------------

  P.at(0, 0) = 0;
  P.at(0, 1) = 0;
  P.at(1, 0) = 0;
  P.at(1, 1) = 0;

  //------------
  C.at(0, 0) = 3;
  C.at(1, 0) = 3;
  C.at(2, 0) = 3;

  MatrixMul(M, N, P);
  std::cout << M << std::endl;
  std::cout << N << std::endl;
  std::cout << P << std::endl;

  std::cout << "------------------------\n";

  MatrixMul(M, C, R);
  std::cout << M << std::endl;
  std::cout << C << std::endl;
  std::cout << R << std::endl;
}

}  // namespace math
