#include <memory.h>
#include <cassert>
#include <cstring>
#include <omp.h>
#include "Matrix.hpp"

Matrix::Matrix(long M, long N, double* _data, bool allocate) {
  if(allocate){
    data = new double[M*N];
    if(_data != nullptr)
      std::memcpy(data, _data, sizeof(double)*static_cast<size_t>(M * N));
    else {
      for (long i = 0; i < M * N; ++i)
        data[i] = 0;
    }
    allocated = true;
  }
  else{
    data = _data;
  }
  _height = M;
  _width = N;
  _stride = 0;
}

Matrix::~Matrix() {
  if(allocated)
   delete[] data;
}

Vector Matrix::operator[](long ind) const{
  return Vector(_width, data + ind *(_width + _stride), false);
}

std::string Matrix::to_string() {
  std::string res = "[ ";
  double *ptr = data;
  for(long i = 0; i < _height; ++i){
    for(long j = 0; j < _width; ++j){
      res += std::to_string(*ptr);
      res += " ";
      ++ptr;
    }
    if (i < _height - 1)
      res += "\n  ";
    ptr += _stride;
  }
  res += "]";
  return res;
}

void Matrix::operator+=(const Matrix &other) {
  assert(_height == other._height);
  for(int i = 0; i < _height; ++i)
    (*this)[i] += other[i];
}

Matrix Matrix::operator+(const Matrix &other) const{
  Matrix res(_width, _height, data, true);
  res += other;
  return res;
}

void Matrix::operator*=(const Matrix &other) {
  assert(_height == other._height);
  for(int i = 0; i < _height; ++i)
    (*this)[i] *= other[i];
}

Matrix Matrix::operator*(const Matrix &other) const{
  Matrix res(_width, _height, data, true);
  res *= other;
  return res;
}

void Matrix::operator*=(double val) {
  for(int i = 0; i < _height; ++i)
    (*this)[i] *= val;
}

Matrix Matrix::operator*(double val) const {
  Matrix res(_height, _width, data, true);
  res *= val;
  return res;
}

Matrix operator*(double val, const Matrix &mat) {
  return mat * val;
}

Matrix::Matrix(const Matrix &mat):Matrix(mat._width, mat._height) {
  for(int i = 0; i < _height; ++i) {
    Vector vec = (*this)[i];
    vec <<= mat[i];
  }
}

Matrix Matrix::getRegion(long M1, long N1, long width, long height) {
  assert(width > 0 && height > 0);
  assert(M1 < _width && M1 + width <= _width);
  assert( N1 < _height && N1 + height <= _height);
  Matrix res(height, width, data + M1 + N1*(_width + _stride), false);
  res._stride = _width - width + _stride;
  return res;
}

bool Matrix::operator==(const Matrix &mat) {
  assert(_height == mat._height);
  for(int i = 0; i < _height; ++i) {
    if (mat[i] != (*this)[i])
      return false;
  }
  return true;
}

Matrix naive_matmul(Matrix& A, Matrix& B){
  assert(A.width() == B.height());
  auto depth = A.width();
  auto width = B.width();
  auto height = A.height();
  Matrix res(height, width);
  for (int i = 0; i < height; ++i){
    Vector v = res[i];
    for (int j = 0; j < width; ++j){
      double &cij = v[j];
      for (int k = 0; k < depth; ++k) {
        cij += A[i][k] * B[k][j];
      }
    }
  }
  return res;
}

Matrix naive_matmul_parallel(Matrix& A, Matrix& B){
  assert(A.width() == B.height());
  auto depth = A.width();
  auto width = B.width();
  auto height = A.height();
  Matrix res(height, width);
#pragma omp parallel for
  for (int i = 0; i < height; ++i){
    Vector v = res[i];
    for (int j = 0; j < width; ++j){
      double &cij = v[j];
      for (int k = 0; k < depth; ++k) {
        cij += A[i][k] * B[k][j];
      }
    }
  }
  return res;
}

