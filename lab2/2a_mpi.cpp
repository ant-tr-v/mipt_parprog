#include <cstdio>
#include <cmath>
#include <iostream>
#include <mpi/mpi.h>
#include <chrono>

const int ISIZE = 1000;
const int JSIZE = 10000;

inline int isize_by_rank(int rank, int size){
  return  (ISIZE - 1)/size + int(rank  < (ISIZE - 1) % size);
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
  double *_b = (rank)? new double[isize * JSIZE] : new double[ISIZE * JSIZE];
  auto b = reinterpret_cast<double (*)[JSIZE]>(_b);
  int i, j;
  FILE *ff;

  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      a[i][j] = JSIZE * i + j;
    }
  }

  auto time_s = std::chrono::high_resolution_clock::now();
  int start = start = start_by_rank(rank, size);
  for (i = 0; i < isize ; i++) {
    int ind = i + start;
    b[i][0] = a[ind][0];
    for (j = 1; j < JSIZE; j++) {
      b[i][j] = sin(0.00001 * a[ind + 1][j - 1]);
    }
  }
  if (rank == 0){
    for (j = 0; j < JSIZE; j++) {
      b[ISIZE - 1][j] = a[ISIZE - 1][j];
    }
  }
  std::cout << rank << " " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - time_s).count() << " us\n";

  if (rank > 0){
    MPI::COMM_WORLD.Send(b, isize*JSIZE, MPI_DOUBLE, 0, rank);
  }
  if (rank == 0) {
    for (int i = 1; i < size; ++i) {
      MPI::COMM_WORLD.Irecv(b[start_by_rank(i, size)],
                            isize_by_rank(i, size)*JSIZE,
                            MPI_DOUBLE, MPI_ANY_SOURCE, i);
    }
  }
  MPI::COMM_WORLD.Barrier();

  if (rank == 0)
  {
    ff = fopen("result_a2m.txt", "w");
    for (i = 0; i < ISIZE; i++) {
      for (j = 0; j < JSIZE; j++) {
        fprintf(ff, "%f ", b[i][j]);
      }
      fprintf(ff, "\n");
    }
    std::cout << "done";
    fclose(ff);
  }
  delete[] a;
  delete[] b;
  MPI::Finalize();
}