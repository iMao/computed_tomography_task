#include "matrix.h"

#include <iomanip>

namespace math {

const double SMALL = 1.0E-30;

Matrix::Matrix(int rows, int cols)
    : rows_(rows), cols_(cols), matrix_(new double[rows * cols]) {}

Matrix::Matrix(const Matrix &matrix)
    : rows_(matrix.rows_), cols_(matrix.cols_) {
  int size = matrix.rows_ * matrix.cols_;
  matrix_ = new double[size];
  for (int j = 0; j < (rows_ * cols_); j++) {
    matrix_[j] = matrix.matrix_[j];
  }
}

Matrix::Matrix(Matrix &&matrix)
    : rows_(matrix.rows_), cols_(matrix.cols_), matrix_(matrix.matrix_) {
  matrix.matrix_ = nullptr;
}

Matrix &Matrix::operator=(const Matrix &matrix) {
  rows_ = matrix.rows_;
  cols_ = matrix.cols_;

  if (matrix_) {
    delete[] matrix_;
  }

  int size = matrix.rows_ * matrix.cols_;
  matrix_ = new double[size];
  for (int j = 0; j < (rows_ * cols_); j++) {
    matrix_[j] = matrix.matrix_[j];
  }
  return *this;
}

Matrix &Matrix::operator=(Matrix &&matrix) {
  rows_ = matrix.rows_;
  cols_ = matrix.cols_;
  matrix_ = matrix.matrix_;
  matrix.matrix_ = nullptr;

  return *this;
}

Matrix::~Matrix() {
  if (matrix_) {
    delete[] matrix_;
  }
}

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
      os << std::setw(5) << m[y * m.cols_ + x] << " ";
    }
    os << std::endl;
  }
  return os;
}

double RoundTooLittleValue(double x) { return (std::abs(x) < 0.0001 ? 0 : x); }

void ComposeMatrixFromClusterLines(const std::vector<unsigned int> &cluster,
                                   const std::vector<tmg::Line2D> &lines,
                                   Matrix &matrix) {
  std::set<unsigned int> set_cluster(cluster.begin(), cluster.end());

  int y = 0;
  int x = 0;
  for (auto &l : lines) {
    if (set_cluster.find(l.GetLineNumber()) != set_cluster.end()) {
      matrix.at(y, x) = l.GetA();
      x++;
      matrix.at(y, x) = l.GetB();
      y++;
      x = 0;
    }
  }
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

bool CreateTransposedMatrix(const Matrix &m, Matrix &transposed_m) {
  if (m.cols_ != transposed_m.rows_ || m.rows_ != transposed_m.cols_) {
    return false;
  }

  for (int y = 0; y < m.rows_; y++) {
    for (int x = 0; x < m.cols_; x++) {
      transposed_m.at(x, y) = m.at(y, x);
    }
  }

  return true;
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

double Determinant(const Matrix &B) {
  int n = B.cols_;

  Matrix A(B);

  double det = 1.0;

  // Row operations for i = 0, ,,,, n - 2 (n-1 not needed)
  for (int i = 0; i < n - 1; i++) {
    // Partial pivot: find row r below with largest element in column i
    int r = i;
    double maxA = std::abs(A.at(i, i));
    for (int k = i + 1; k < n; k++) {
      double val = std::abs(A.at(k, i));
      if (val > maxA) {
        r = k;
        maxA = val;
      }
    }
    if (r != i) {
      for (int j = i; j < n; j++) {
        std::swap(A.at(i, j), A.at(r, j));
      }
      det = -det;
    }

    // Row operations to make upper-triangular
    double pivot = A.at(i, i);
    // check if Singular matrix
    if (abs(pivot) < SMALL) {
      return 0.0;
    }

    // On lower rows
    for (int r = i + 1; r < n; r++) {
      // Multiple of row i to clear element in ith column
      double multiple = A.at(r, i) / pivot;
      for (int j = i; j < n; j++) {
        A.at(r, j) -= multiple * A.at(i, j);
      }
    }
    det *= pivot;  // Determinant is product of diagonal
  }

  det *= A.at(n - 1, n - 1);

  return det;
}

bool CramerRuleSolver(const Matrix &A, const Matrix &vecB, Matrix &vecX) {
  if ((A.cols_ != A.rows_) || (vecB.rows_ != A.rows_) ||
      (vecX.rows_ != A.rows_)) {
    return false;
  }

  std::vector<math::Matrix> augmented_matricesA;

  // matrix formation
  for (int i = 0; i < A.cols_; i++) {
    augmented_matricesA.push_back(A);
  }

  // column substitution
  for (int i = 0; i < A.cols_; i++) {
    for (int j = 0; j < A.rows_; j++) {
      augmented_matricesA[i].at(j, i) = vecB.at(j, 0);
    }
  }

  double detA = Determinant(A);

  // calculation of vector X values
  for (int k = 0; k < A.cols_; k++) {
    vecX.at(k, 0) = Determinant(augmented_matricesA[k]) / detA;
  }

  return true;
}

//-------------------------Tests---------------------------------------
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

void TestTransposeMatrix() {
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

  Matrix TM3x3(3, 3);
  CreateTransposedMatrix(M3x3, TM3x3);

  std::cout << "Matrix 3x3:            " << M3x3 << std::endl;
  std::cout << "Transposed Matrix 3x3: " << TM3x3 << std::endl;

  Matrix M3x2(3, 2);
  M3x2.at(0, 0) = 3.0;
  M3x2.at(1, 0) = 1.0;
  M3x2.at(2, 0) = 4.0;

  M3x2.at(0, 1) = 2.0;
  M3x2.at(1, 1) = 1.0;
  M3x2.at(2, 1) = 2.0;

  Matrix TM2x3(2, 3);
  CreateTransposedMatrix(M3x2, TM2x3);

  std::cout << "Matrix 3x2:            " << M3x2 << std::endl;
  std::cout << "Transposed Matrix 2x3: " << TM2x3 << std::endl;
}

void TestCramerRuleSolver() {
  Matrix A3x3(3, 3);

  A3x3.at(0, 0) = 2.0;
  A3x3.at(1, 0) = 1.0;
  A3x3.at(2, 0) = 1.0;

  A3x3.at(0, 1) = 1.0;
  A3x3.at(1, 1) = -1.0;
  A3x3.at(2, 1) = 2.0;

  A3x3.at(0, 2) = 1.0;
  A3x3.at(1, 2) = -1.0;
  A3x3.at(2, 2) = 1.0;

  Matrix B3x1(3, 1);
  B3x1.at(0, 0) = 3.0;
  B3x1.at(1, 0) = 0.0;
  B3x1.at(2, 0) = 0.0;

  Matrix X3x1(3, 1);
  X3x1.at(0, 0) = 0.0;
  X3x1.at(1, 0) = 0.0;
  X3x1.at(2, 0) = 0.0;

  if (CramerRuleSolver(A3x3, B3x1, X3x1)) {
    std::cout << "Solution of linear system\n";
    std::cout << "Matrix A 3x3: " << A3x3 << std::endl;
    std::cout << "Matrix B 3x1: " << B3x1 << std::endl;
    std::cout << "Matrix X 3x1: " << X3x1 << std::endl;
  } else {
    std::cout << "Wrong params of linear system\n";
  }
}

void TestConstructors() {
  std::vector<math::Matrix> matrices;

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

  for (int i = 0; i < 5; i++) {
    matrices.push_back(M3x3);
  }

  for (int i = 0; i < 5; i++) {
    std::cout << matrices[i] << std::endl;
  }
}

}  // namespace math
