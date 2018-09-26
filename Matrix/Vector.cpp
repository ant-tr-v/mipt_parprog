#include <memory.h>
#include <cassert>
#include <cstring>
#include "Vector.hpp"

Vector::Vector(long M, double *_data, bool allocate) {
  if(allocate){
    data = new double[M];
    if(_data)
      std::memcpy(data, _data, sizeof(double)*static_cast<size_t>(M));
    else {
      for (long i = 0; i < M; ++i)
        data[i] = 0;
    }
    allocated = true;
  }
  else{
    data = _data;
  }
  _size = M;
}

Vector::~Vector() {
  if(allocated){
    delete[] data;
  }
}

void Vector::operator+=(const Vector &other) {
  assert(_size == other._size);
  for(int i = 0; i < _size; ++i){
    data[i] += other.data[i];
  }
}

void Vector::operator*=(const Vector &other) {
  assert(_size == other._size);
  for(int i = 0; i < _size; ++i){
    data[i] *= other.data[i];
  }
}


void Vector::operator*=(double val) {
  for(int i = 0; i < _size; ++i){
    data[i] *= val;
  }
}

Vector Vector::operator+(const Vector &other) const{
  assert(_size == other._size);
  Vector res(_size, data, true);
  for(int i = 0; i < _size; ++i){
    res.data[i] += other.data[i];
  }
  return res;
}

Vector Vector::operator*(const Vector &other) const{
  assert(_size == other._size);
  Vector res(_size, data, true);
  for(int i = 0; i < _size; ++i){
    res.data[i] *= other.data[i];
  }
  return res;
}

Vector Vector::operator*(double val) const{
  Vector res(_size, data, true);
  for(int i = 0; i < _size; ++i){
    res.data[i] *= val;
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
    res += data[i]*other.data[i];
  }
  return res;
}

double dot(const Vector &vec1, const Vector &vec2) {
  return vec1.dot(vec2);
}

void Vector::operator<<=(const Vector &vec) {
  assert(_size == vec._size);
  memccpy(data, vec.data, sizeof(double), static_cast<size_t>(_size));
}

bool Vector::operator==(const Vector &vec) {
  assert(_size == vec._size);
  for(int i = 0; i < _size; ++i){
    if ((data[i] - vec.data[i]) > 1e-9)
      return false;
  }
  return true;
}

bool Vector::operator!=(const Vector &vec) {
  assert(_size == vec._size);
  return !((*this) == vec);
}
