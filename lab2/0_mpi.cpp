#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <iostream>
#include <mpi/mpi.h>

#define ISIZE 1000
#define JSIZE 1000

inline int isize_by_rank(int rank, int size){
  return  (ISIZE)/size + int(rank  < (ISIZE) % size);
}

inline int start_by_rank(int rank, int size){
  int res = 0;
  for(int i = 0; i < rank; ++i){
    res += isize_by_rank(i, size);
  }
  return res;
}

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  const int rank =  MPI::COMM_WORLD.Get_rank();
  const int size = MPI::COMM_WORLD.Get_size();
  const int isize = isize_by_rank(rank, size);

  double *_a = new double[ISIZE * JSIZE];
  auto a = reinterpret_cast<double (*)[JSIZE]>(_a);
  FILE *ff;
  for (int i = 0; i < ISIZE ; i++) {
    for (int j = 0; j < JSIZE; j++) {
      a[i][j] = JSIZE * i + j;
    }
  }
  auto time_s = std::chrono::high_resolution_clock::now();
  int start = start = start_by_rank(rank, size);

  for (int i = start; i < start + isize; i++) {
    for (int j = 0; j < JSIZE; j++) {
      a[i][j] = sin(0.00001 * a[i][j]);
    }
  }
  std::cout << rank << " " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - time_s).count() << " us\n";

  if (rank > 0){
    MPI::COMM_WORLD.Send(a + start, isize*JSIZE, MPI_DOUBLE, 0, rank);
  }
  if (rank == 0) {
    for (int i = 1; i < size; ++i) {
      MPI::COMM_WORLD.Irecv(a[start_by_rank(i, size)],
                            isize_by_rank(i, size)*JSIZE,
                            MPI_DOUBLE, MPI_ANY_SOURCE, i);
    }
  }
  MPI::COMM_WORLD.Barrier();

  if (rank == 0)
  {
    ff = fopen("result_0m.txt","w");
    for (int i = 0; i < ISIZE; i++) {
      for (int j = 0; j < JSIZE; j++) {
        fprintf(ff, "%f ", a[i][j]);
      }
      fprintf(ff, "\n");
    }
    fclose(ff);
  }
  delete[] a;
  MPI::Finalize();
}