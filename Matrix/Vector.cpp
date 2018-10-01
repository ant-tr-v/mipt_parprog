#include <memory.h>
#include <cassert>
#include <cstring>
#include <algorithm>
#include "Vector.hpp"

Vector::Vector(long M, double *data_ptr, bool allocate) {
  if(allocate){
    _data = new double[M];
    if(data_ptr)
      std::memcpy(_data, data_ptr, sizeof(double)*static_cast<size_t>(M));
    else {
      for (long i = 0; i < M; ++i)
        _data[i] = 0;
    }
    allocated = true;
  }
  else{
    _data = data_ptr;
  }
  _size = M;
}

Vector::~Vector() {
  if(allocated){
    delete[] _data;
  }
}

void Vector::operator+=(const Vector &other) {
  assert(_size == other._size);
  for(int i = 0; i < _size; ++i){
    _data[i] += other._data[i];
  }
}

void Vector::operator-=(const Vector &other) {
  assert(_size == other._size);
  for(int i = 0; i < _size; ++i){
    _data[i] -= other._data[i];
  }
}

void Vector::operator*=(const Vector &other) {
  assert(_size == other._size);
  for(int i = 0; i < _size; ++i){
    _data[i] *= other._data[i];
  }
}


void Vector::operator*=(double val) {
  for(int i = 0; i < _size; ++i){
    _data[i] *= val;
  }
}

Vector Vector::operator+(const Vector &other) const{
  assert(_size == other._size);
  Vector res(_size, _data, true);
  for(int i = 0; i < _size; ++i){
    res._data[i] += other._data[i];
  }
  return res;
}

Vector Vector::operator-(const Vector &other) const{
  assert(_size == other._size);
  Vector res(_size, _data, true);
  for(int i = 0; i < _size; ++i){
    res._data[i] -= other._data[i];
  }
  return res;
}

Vector Vector::operator*(const Vector &other) const{
  assert(_size == other._size);
  Vector res(_size, _data, true);
  for(int i = 0; i < _size; ++i){
    res._data[i] *= other._data[i];
  }
  return res;
}

Vector Vector::operator*(double val) const{
  Vector res(_size, _data, true);
  for(int i = 0; i < _size; ++i){
    res._data[i] *= val;
  }
  return res;
}


Vector operator*(double val, const Vector&  vec) {
  return vec * val;
}

double Vector::dot(const Vector &other) const {
  assert(_size == other._size);
  double res = 0;
  for(int i = 0; i < _size; ++i){
    res += _data[i]*other._data[i];
  }
  return res;
}

double dot(const Vector &vec1, const Vector &vec2) {
  return vec1.dot(vec2);
}

void Vector::operator<<=(const Vector &vec) {
  long sz = std::min(_size, vec._size);
  std::memcpy(_data, vec._data, sizeof(double)*static_cast<size_t>(sz));
}

bool Vector::operator==(const Vector &vec) {
  assert(_size == vec._size);
  for(int i = 0; i < _size; ++i){
    if ((_data[i] - vec._data[i]) > eps)
      return false;
  }
  return true;
}

bool Vector::operator!=(const Vector &vec) {
  assert(_size == vec._size);
  return !((*this) == vec);
}
