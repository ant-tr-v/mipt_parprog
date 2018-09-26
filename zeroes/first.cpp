#include <iostream>
#include <omp.h>


int main(int argc, char *argv[]) {
  long N = 8;
  if (argc > 1){
    N = strtol(argv[1], nullptr, 10);
  }
  omp_set_num_threads(static_cast<int>(N));
#pragma omp parallel
  {
    std::cout <<"Hello"<<omp_get_thread_num()<< std::endl;
  };

  return 0;
}