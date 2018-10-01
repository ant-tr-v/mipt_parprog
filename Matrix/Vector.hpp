//
// Created by anton on 14.09.18.
//

#ifndef OMP_VECTOR_HPP
#define OMP_VECTOR_HPP


class Vector {
  double *_data = nullptr;
  long _size = 0;
  bool allocated = false;
  const double eps = 1e-9;
public:
  double& operator[](long ind){
    return _data[ind];
  };
  Vector(){;}
  explicit Vector(long M,  double* data_ptr=nullptr, bool allocate=false);
  Vector (const Vector& vec):Vector(vec._size, vec._data, true){}; //delegated constructor, all hail C++11))
  long size(){
    return _size;
  }
  double * data() { return _data;};
  void operator += (const Vector& other);
  void operator -= (const Vector& other);
  void operator *= (const Vector& other);
  void operator *= (double val);
  Vector operator + (const Vector& other) const;
  Vector operator - (const Vector& other) const;
  Vector operator * (const Vector& other) const;
  Vector operator * (double val) const;
  void operator <<= (const Vector& vec); // copy into this vector, no allocation
  friend Vector operator *(double val, const Vector&  vec);
  friend double dot(const Vector& vec1, const Vector&  vec2);
  double dot(const Vector& other) const;
  bool operator == (const Vector& vec);
  bool operator != (const Vector& vec);
  ~Vector();

  };


#endif //OMP_VECTOR_HPP
