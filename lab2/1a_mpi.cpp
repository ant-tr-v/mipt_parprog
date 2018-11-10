#include <cstdio>
#include <cmath>
#include <iostream>
#include <mpi/mpi.h>
#include <chrono>
#include <vector>

const int ISIZE = 5000;
const int JSIZE = 5000;

inline int line_size(int p){
  int a  = std::min(ISIZE, JSIZE);
  int b  = std::max(ISIZE, JSIZE);
  return (p < a)? p + 1 : (p < b)? a : (a + b - 1 - p);
}

void jpoints_by_rank(std::vector<int>&starts, std::vector<int>&sizes, int size){
  starts[0] = 0;
  int total = 0;
  int sum = ISIZE * JSIZE;
  for(int i = 1, p = 0; i < size; ++i){
    int k = 0;
    while(k < sum/size){
      k += line_size(p);
      ++p;
    }
    starts[i] = p;
    sizes[i - 1] = k;
    total += k;
  }
  sizes[size - 1] =  sum - total;
  starts[size] = ISIZE + JSIZE - 1;
}

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  const int rank =  MPI::COMM_WORLD.Get_rank();
  const int size = MPI::COMM_WORLD.Get_size();

  std::vector<int> starts(size + 1), sizes(size);
  jpoints_by_rank(starts, sizes, size);

  double *b = new double[sizes[rank]];
  FILE *ff;

  auto time_s = std::chrono::high_resolution_clock::now();
  double *dst = b;
  for(int s = starts[rank]; s < starts[rank + 1]; ++s){
    int j = (s < JSIZE)? s : JSIZE - 1;
    int i = (s < JSIZE)? 0 : s + 1 - JSIZE;
    *dst = JSIZE * i + j; // we don't need a actually
    double *ptr = dst;
    for(--j, ++i, ++dst; j >= 0 && i < ISIZE; --j, ++i){
      *dst =sin(0.00001 * (*ptr));
      ptr = dst;
      ++dst;
    }
  }
  std::cout <<  rank <<" "<< std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - time_s).count() << " us\n";

  if (rank > 0){
    MPI::COMM_WORLD.Send(b, sizes[rank], MPI_DOUBLE, 0, rank);
  }
  if (rank == 0) {

    double *_a = new double[ISIZE * JSIZE];
    auto a = reinterpret_cast<double (*)[JSIZE]>(_a);
    for (int k = 0; k < size; ++k) {
      double *tmp =(k)? new double[sizes[k]]: b;
      if(k) {
        MPI::COMM_WORLD.Recv(tmp, sizes[k],
                             MPI_DOUBLE, MPI_ANY_SOURCE, k);
      }
      double *src = tmp;
      int m = 0;
      for(int s = starts[k]; s < starts[k + 1]; ++s){
        int j = (s < JSIZE)? s : JSIZE - 1;
        int i = (s < JSIZE)? 0 : s + 1 - JSIZE;
        for(; j >= 0 && i < ISIZE; --j, ++i){
          a[i][j] = *src;
          ++src;
        }
      }
      if(k)
        delete[] tmp;
    }

    ff = fopen("result_a1m.txt", "w");
//    for (int i = 0; i < ISIZE; i++) {
//      for (int j = 0; j < JSIZE; j++) {
//        fprintf(ff, "%f ", a[i][j]);
//      }
//      fprintf(ff, "\n");
//    }
    fclose(ff);
    delete[] a;
  }
  delete[] b;
  MPI::Finalize();
}