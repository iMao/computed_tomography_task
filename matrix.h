#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <memory>
#include <set>
#include <vector>

#include "lines.h"

namespace math {

/**
 * @brief The Matrix struct
 */
struct Matrix {
  Matrix() = delete;
  Matrix(int rows, int cols);
  Matrix(const Matrix& matrix);
  Matrix(Matrix&& matrix);
  Matrix& operator=(const Matrix& matrix);
  Matrix& operator=(Matrix&& matrix);
  ~Matrix();

  double& at(unsigned int y, unsigned int x);
  const double& at(unsigned int y, unsigned int x) const;

  double& operator[](unsigned int i);
  const double& operator[](unsigned int i) const;

  int rows_;
  int cols_;
  double* matrix_;
};

std::ostream& operator<<(std::ostream& os, Matrix& m);

/**
 * @brief RoundTooLittleValue
 * @param x
 * @return
 */
double RoundTooLittleValue(double x);

/**
 * @brief ComposeMatrixFromClusterLines
 * @param cluster
 * @param lines
 * @param m
 */
void ComposeMatrixFromClusterLines(const std::vector<unsigned int>& cluster,
                                   const std::vector<tmg::Line2D>& lines,
                                   math::Matrix& matrix);

/**
 * @brief ComposeColumnVector
 * @param cluster
 * @param lines
 * @param column
 */
void ComposeColumnVector(const std::vector<unsigned int>& cluster,
                         const std::vector<tmg::Line2D>& lines,
                         std::vector<double>& column);
/**
 * @brief ComposeColumnVector
 * @param cluster
 * @param lines
 * @param column
 */
void ComposeColumnVector(const std::vector<unsigned int>& cluster,
                         const std::vector<tmg::Line2D>& lines, Matrix& column);
/**
 * @brief CreateTransposedMatrix
 * @param m
 * @param transposed_m
 * @return
 */
bool CreateTransposedMatrix(const Matrix& m, Matrix& transposed_m);

/**
 * @brief MatrixMul
 * @param M
 * @param N
 * @param P
 * @return
 */
bool MatrixMul(const Matrix& M, const Matrix& N, Matrix& P);

/**
 * @brief Determinant
 * @param B
 * @return
 */
double Determinant(const math::Matrix& B);

/**
 * @brief CramerRuleSolver to solve linear system of equation
 * @param A - square matrix
 * @param B - right part vector
 * @param X - vector of unknown variables
 */
bool CramerRuleSolver(const math::Matrix& A, const math::Matrix& vecB,
                      math::Matrix& vecX);

//----------------------Tests--------------------------

/**
 * @brief TestMatrixMul
 */
void TestMatrixMul();

/**
 * @brief TestDeterminant
 */
void TestDeterminant();

/**
 * @brief TestTransposeMatrix
 */
void TestTransposeMatrix();

/**
 * @brief TestCramerRuleSolver
 */
void TestCramerRuleSolver();

/**
 * @brief TestConstructors
 */
void TestConstructors();

}  // namespace math

#endif  // MATRIX_H
