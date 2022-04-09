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

bool MatrixMul(const Matrix &M, const Matrix &N, Matrix &P) {
  if (M.cols_ != N.rows_) {
    return false;
  }

  if (P.rows_ != M.rows_ || P.cols_ != N.cols_) {
    return false;
  }

  for (int i = 0; i < M.rows_; i++) {
    for (int j = 0; j < N.cols_; j++) {
      for (int k = 0; k < M.cols_; k++) {
        P.at(i, j) += M.at(i, k) * N.at(k, j);
      }
    }
  }
  return true;
}

double determinant(double *matrix, int n) {
  double det = 0.0;
  double *submatrix = new double[n * n];
  if (n == 2) {
    double det2x2 = ((matrix[0 * n + 0] * matrix[1 * n + 1]) -
                     (matrix[1 * n + 0] * matrix[0 * n + 1]));

    delete[] submatrix;
    return det2x2;
  } else {
    for (int x = 0; x < n; x++) {
      int subi = 0;
      for (int i = 1; i < n; i++) {
        int subj = 0;
        for (int j = 0; j < n; j++) {
          if (j == x) {
            continue;
          }
          submatrix[subi * n + subj] = matrix[i * n + j];
          subj++;
        }
        subi++;
      }
      det = det +
            (pow(-1, x) * matrix[0 * n + x] * determinant(submatrix, n - 1));
    }
  }

  delete[] submatrix;
  return det;
}

double Determinant(Matrix &matrix) {
  return determinant(matrix.matrix_.get(), matrix.rows_);
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

void TestDeterminant() {
  Matrix M3x3(3, 3);
  M3x3.at(0, 0) = 3.0;
  M3x3.at(1, 0) = 1.0;
  M3x3.at(2, 0) = 4.0;

  M3x3.at(0, 1) = 2.0;
  M3x3.at(1, 1) = 1.0;
  M3x3.at(2, 1) = 2.0;

  M3x3.at(0, 2) = 1.0;
  M3x3.at(1, 2) = 2.0;
  M3x3.at(2, 2) = 3.0;

  Matrix M2x2(2, 2);
  M2x2.at(0, 0) = 4.0;
  M2x2.at(1, 0) = 2.0;

  M2x2.at(0, 1) = 3.0;
  M2x2.at(1, 1) = 1.0;

  double detM3x3 = Determinant(M3x3);
  double detM2x2 = Determinant(M2x2);

  std::cout << M3x3 << std::endl;
  std::cout << M2x2 << std::endl;

  std::cout << "det3x3: " << detM3x3 << std::endl;
  std::cout << "det2x2: " << detM2x2 << std::endl;
}

}  // namespace math
