//
// Created by anton on 14.09.18.
//

#ifndef OMP_VECTOR_HPP
#define OMP_VECTOR_HPP


class Vector {
  double *data = nullptr;
  long _size = 0;
  bool allocated = false;
public:
  double& operator[](long ind){
    return data[ind];
  };
  Vector(){;}
  explicit Vector(long M,  double* _data=nullptr, bool allocate=false);
  Vector (const Vector& vec):Vector(vec._size, vec.data, true){}; //delegated constructor, all hail C++11))
  long size(){
    return _size;
  }
  void operator += (const Vector& other);
  void operator *= (const Vector& other);
  void operator *= (double val);
  Vector operator + (const Vector& other) const;
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
