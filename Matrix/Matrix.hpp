//
// Created by anton on 14.09.18.
//

#ifndef OMP_MATRIX_HPP
#define OMP_MATRIX_HPP

#include <string>
#include "Vector.hpp"

class Matrix {
  bool allocated = false;
  double *data = nullptr;
  long _width = 0;
  long _height = 0;
  long _stride = 0;
public:
  Matrix(){;}
  Matrix(long M, long N, double* _data= nullptr, bool allocate=true);
  Vector operator[](long ind) const;
  std::string to_string() const;
  long width() const{
    return _width;
  };
  long height() const{
    return _height;
  };
  long stride() const{
    return _stride;
  };
  inline long size() const{
    return _width*_height;
  }
  void operator += (const Matrix& other);
  Matrix operator + (const Matrix& other) const;
  void operator -= (const Matrix& other);
  Matrix operator - (const Matrix& other) const;
  void operator *= (const Matrix& other);
  Matrix operator * (const Matrix& other) const;
  void operator *= (double val);
  Matrix operator * (double val) const;
  friend Matrix operator *(double val, const Matrix&  mat);
  Matrix getRegion(long M1, long N1, long width, long height);
  Matrix reshape(long M, long N);
  Matrix(const Matrix& mat);
  bool operator == (const Matrix& mat);
  friend Matrix transpose(Matrix &Mat);
  ~Matrix();
};


Matrix naive_matmul(Matrix& A, Matrix& B);
Matrix naive_matmul_parallel(Matrix& A, Matrix& B);
Matrix transposed_matmul(Matrix& A, Matrix& B);
Matrix transposed_matmul_parallel(Matrix& A, Matrix& B);
Matrix strassen_matmul(Matrix &A, Matrix &B);
Matrix strassen_matmul_parallel(Matrix &A, Matrix &B);

#endif //OMP_MATRIX_HPP
