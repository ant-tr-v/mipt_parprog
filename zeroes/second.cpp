#include <iostream>
#include <omp.h>
#include <vector>


const int N = 100;

int main(int argc, char *argv[]) {
  long N = 8;
  if (argc > 1){
    N = strtol(argv[1], nullptr, 10);
  }
  std::vector<omp_lock_t> lock(static_cast<unsigned long>(N));
  for (int i = 0; i < N; ++i){
    omp_init_lock(lock.data() + i);
    omp_set_lock(lock.data() + i);
  }
  omp_set_num_threads(N);
#pragma omp parallel
  {
    int t = omp_get_thread_num();
    if(t)
      omp_set_lock(lock.data() + t);
    std::cout << t << std::endl;
    if(t < N - 1)
      omp_unset_lock(lock.data() + t + 1);
  };

  return 0;
}