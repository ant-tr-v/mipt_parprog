#include <iostream>
#include <iomanip>
#include <limits>
#include <chrono>
#include <omp.h>


int main(int argc, char *argv[]) {
  long N = 2000000000;
  if (argc > 1){
    N = strtol(argv[1], nullptr, 10);
  }
  double sum = 0;
  auto start = std::chrono::system_clock::now();
#pragma omp parallel for reduction(+:sum)
for(long i = 1; i < N; i++){
    if(i%2)
      sum += 1./(2 * i - 1);
    else
      sum += -1./(2 * i - 1);
  }
  std::cout << std::setprecision(10) << sum *4 <<"\n";
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() << "ms\n";
  return 0;
}