#include <cstdio>
#include <cmath>
#include <chrono>
#include <iostream>
const int ISIZE = 3000;
const int JSIZE = 3000;

int main(int argc, char **argv) {
  double *_a = new double[ISIZE * JSIZE];
  auto a = reinterpret_cast<double (*)[JSIZE]>(_a);
  int i, j;
  FILE *ff;
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      a[i][j] = JSIZE * i + j;
    }
  }
  auto time_s = std::chrono::high_resolution_clock::now();
  for (i = 0; i < ISIZE - 1; i++) {
    for (j = 1; j < JSIZE; j++) {
      a[i][j] = sin(0.00001 * a[i + 1][j - 1]);
    }
  }
  std::cout << "baseline " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - time_s).count() << " us\n";
  ff = fopen("result_a2b.txt", "w");
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      fprintf(ff, "%f ", a[i][j]);
    }
    fprintf(ff, "\n");
  }
  fclose(ff);
  delete[] a;
}