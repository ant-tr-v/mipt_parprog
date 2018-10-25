#include <cstdio>
#include <cmath>

const int ISIZE = 1000;
const int JSIZE = 100;

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
  for (i = 0; i < ISIZE - 1; i++) {
    for (j = 1; j < JSIZE; j++) {
      a[i][j] = sin(0.00001 * a[i + 1][j - 1]);
    }
  }
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