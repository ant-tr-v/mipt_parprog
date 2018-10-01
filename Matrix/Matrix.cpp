#include <memory.h>
#include <cassert>
#include <cstring>
#include <omp.h>
#include <iostream>
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

std::string Matrix::to_string() const{
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
  Matrix res = *this;
  res += other;
  return res;
}

void Matrix::operator-=(const Matrix &other) {
  assert(_height == other._height);
  for(int i = 0; i < _height; ++i)
    (*this)[i] -= other[i];
}

Matrix Matrix::operator-(const Matrix &other) const{
  Matrix res = *this;
  res -= other;
  return res;
}

void Matrix::operator*=(const Matrix &other) {
  assert(_height == other._height);
  for(int i = 0; i < _height; ++i)
    (*this)[i] *= other[i];
}

Matrix Matrix::operator*(const Matrix &other) const{
  Matrix res = *this;
  res *= other;
  return res;
}

void Matrix::operator*=(double val) {
  for(int i = 0; i < _height; ++i)
    (*this)[i] *= val;
}

Matrix Matrix::operator*(double val) const {
  Matrix res = *this;
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

Matrix transpose(Matrix &Mat) {
  Matrix res;
  res.data = new double[Mat._height* Mat._height];
  res.allocated = true;
  res._height = Mat._width;
  res._width = Mat._height;
  res._stride = 0;
  for(long i = 0; i < Mat._height; ++i){
    Vector vec = Mat[i];
    for(long j = 0; j < Mat._width; ++j){
      res[j][i] = vec[j];
    }
  }
  return res;
}

Matrix Matrix::reshape(long M, long N) {
  Matrix res(M, N);
  long H = std::min(M, _height);
  long W = std::min(N, _width);
  for(long i = 0; i < H; ++i){
    Vector v = (*this)[i];
    res[i] <<= v;
  }
  return res;
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

Matrix transposed_matmul(Matrix& A, Matrix& B){
  assert(A.width() == B.height());
  Matrix T = transpose(B);
  auto width = B.width();
  auto height = A.height();
  Matrix res(height, width);
  for (int i = 0; i < height; ++i){
    Vector v = res[i];
    Vector a = A[i];
    for (int j = 0; j < width; ++j){
      v[j] = a.dot(T[j]);
    }
  }
  return res;
}

Matrix transposed_matmul_parallel(Matrix& A, Matrix& B){
  assert(A.width() == B.height());
  Matrix T = transpose(B);
  auto width = B.width();
  auto height = A.height();
  Matrix res(height, width);
#pragma omp parallel for
  for (int i = 0; i < height; ++i){
    Vector v = res[i];
    Vector a = A[i];
    for (int j = 0; j < width; ++j){
      v[j] = a.dot(T[j]);
    }
  }
  return res;
}

long xn2(long val){
  long k = 1;
  while (k < val)
    k *= 2;
  return k;
}

Matrix strassen_matmul(Matrix &A, Matrix &B) {
  assert(A.width() == B.height());
  auto width = B.width();
  auto height = A.height();
  long as = std::max(xn2(A.width()), xn2(A.height()));
  long bs = std::max(xn2(B.width()), xn2(B.height()));
  long s = std::max(as, bs);
  if(A.height() != s || A.width() != s || B.height() != s || B.width() != s){
    Matrix A1 = A.reshape(s, s);
    Matrix B1 = B.reshape(s, s);
    return strassen_matmul(A1, B1).reshape(height, width);
  }
  if (s == 1){
    double res[1] = {A[0][0] * B[0][0]};
    return Matrix(1, 1, res);
  }
  Matrix A11 = A.getRegion(0, 0, s/2, s/2);
  Matrix A21 = A.getRegion(0, s/2, s/2, s/2);
  Matrix A12 = A.getRegion(s/2, 0, s/2, s/2);
  Matrix A22 = A.getRegion(s/2, s/2, s/2, s/2);

  Matrix B11 = B.getRegion(0, 0, s/2, s/2);
  Matrix B21 = B.getRegion(0, s/2, s/2, s/2);
  Matrix B12 = B.getRegion(s/2, 0, s/2, s/2);
  Matrix B22 = B.getRegion(s/2, s/2, s/2, s/2);

  Matrix T1 = A11 + A22;
  Matrix T2 = B11 + B22;
  Matrix T3 = A21 + A22;
  Matrix T4 = B12 - B22;
  Matrix T5 = B21 - B11;
  Matrix T6 = A11 + A12;
  Matrix T7 = (A21 - A11);
  Matrix T8 = (B11 + B12);
  Matrix T9 = (A12 - A22);
  Matrix T10 = (B21 + B22);

  Matrix M1 = strassen_matmul(T1, T2);
  Matrix M2 = strassen_matmul(T3, B11);
  Matrix M3 = strassen_matmul(A11, T4);
  Matrix M4 = strassen_matmul(A22, T5);
  Matrix M5 = strassen_matmul(T6, B22);
  Matrix M6 = strassen_matmul(T7, T8);
  Matrix M7 = strassen_matmul(T9, T10);

  Matrix C11 = M1 + M4 - M5 + M7;
  Matrix C12 = M3 + M5;
  Matrix C21 = M2 + M4;
  Matrix C22 = M1 - M2 + M3 + M6;
//
//  std::cout <<"*"<< A.to_string() <<"\n*" << B.to_string() << "\n";
//
//  std::cout << C11.to_string() <<"\n" << C12.to_string() << "\n";
//  std::cout << C21.to_string() <<"\n" << C22.to_string() << "\n";

  Matrix C(height, width);
  for (long i = 0; i < s/2; ++i){
    Vector v1 = C11[i];
    Vector v2 = C12[i];
    C[i] <<= v1;
    Vector v(width - s/2, C[i].data() + s/2, false);
    v <<= v2;
  }

  for (long i = s/2; i < height; ++i){
    Vector v1 = C21[i - s/2];
    Vector v2 = C22[i - s/2];
    C[i] <<= v1;
    Vector v(width - s/2, C[i].data() + s/2, false);
    v <<= v2;
  }

  return C;
}

Matrix strassen_matmul_parallel(Matrix &A, Matrix &B) {
  assert(A.width() == B.height());
  auto width = B.width();
  auto height = A.height();
  long as = std::max(xn2(A.width()), xn2(A.height()));
  long bs = std::max(xn2(B.width()), xn2(B.height()));
  long s = std::max(as, bs);
  if(A.height() != s || A.width() != s || B.height() != s || B.width() != s){
    Matrix A1 = A.reshape(s, s);
    Matrix B1 = B.reshape(s, s);
    return strassen_matmul_parallel(A1, B1).reshape(height, width);
  }
  if (s == 1){
    double res[1] = {A[0][0] * B[0][0]};
    return Matrix(1, 1, res);
  }
  Matrix A11 = A.getRegion(0, 0, s/2, s/2);
  Matrix A21 = A.getRegion(0, s/2, s/2, s/2);
  Matrix A12 = A.getRegion(s/2, 0, s/2, s/2);
  Matrix A22 = A.getRegion(s/2, s/2, s/2, s/2);

  Matrix B11 = B.getRegion(0, 0, s/2, s/2);
  Matrix B21 = B.getRegion(0, s/2, s/2, s/2);
  Matrix B12 = B.getRegion(s/2, 0, s/2, s/2);
  Matrix B22 = B.getRegion(s/2, s/2, s/2, s/2);

  Matrix T1 = A11 + A22;
  Matrix T2 = B11 + B22;
  Matrix T3 = A21 + A22;
  Matrix T4 = B12 - B22;
  Matrix T5 = B21 - B11;
  Matrix T6 = A11 + A12;
  Matrix T7 = (A21 - A11);
  Matrix T8 = (B11 + B12);
  Matrix T9 = (A12 - A22);
  Matrix T10 = (B21 + B22);


  Matrix M1(s/2, s/2);
  Matrix M2(s/2, s/2);
  Matrix M3(s/2, s/2);
  Matrix M4(s/2, s/2);
  Matrix M5(s/2, s/2);
  Matrix M6(s/2, s/2);
  Matrix M7(s/2, s/2);
//  omp_set_num_threads(7);
#pragma omp parallel
  {
    int tr = omp_get_num_threads();
    int tn = omp_get_thread_num();
    if(tn == 0)
      M1 += strassen_matmul(T1, T2);
    if(omp_get_thread_num() == 1 % tr)
      M2 += strassen_matmul(T3, B11);
    if(omp_get_thread_num() == 2 % tr)
      M3 += strassen_matmul(A11, T4);
    if(omp_get_thread_num() == 3 % tr)
      M4 += strassen_matmul(A22, T5);
    if(omp_get_thread_num() == 4 % tr)
      M5 += strassen_matmul(T6, B22);
    if(omp_get_thread_num() == 5 % tr)
      M6 += strassen_matmul(T7, T8);
    if(omp_get_thread_num() == 6 % tr)
     M7 += strassen_matmul(T9, T10);
  }

 // omp_set_num_threads(tn);
  Matrix C11 = M1 + M4 - M5 + M7;
  Matrix C12 = M3 + M5;
  Matrix C21 = M2 + M4;
  Matrix C22 = M1 - M2 + M3 + M6;

//  std::cout <<"*"<< A.to_string() <<"\n*" << B.to_string() << "\n";
//
//  std::cout << C11.to_string() <<"\n" << C12.to_string() << "\n";
//  std::cout << C21.to_string() <<"\n" << C22.to_string() << "\n";

  Matrix C(height, width);
  for (long i = 0; i < s/2; ++i){
    Vector v1 = C11[i];
    Vector v2 = C12[i];
    C[i] <<= v1;
    Vector v(width - s/2, C[i].data() + s/2, false);
    v <<= v2;
  }

  for (long i = s/2; i < height; ++i){
    Vector v1 = C21[i - s/2];
    Vector v2 = C22[i - s/2];
    C[i] <<= v1;
    Vector v(width - s/2, C[i].data() + s/2, false);
    v <<= v2;
  }

  return C;
}