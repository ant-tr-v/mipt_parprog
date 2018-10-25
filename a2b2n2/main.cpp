#include <iostream>
#include <iomanip>
#include <limits>
#include <chrono>
#include <omp.h>
#include <vector>
#include <set>
#include <fstream>

int main(int argc, char *argv[]) {
  long N = 5000;
  if (argc > 1){
    N = strtol(argv[1], nullptr, 10);
  }
  std::vector<std::set<std::pair<long, long>  > > vec(
      static_cast<unsigned long>(2 * N * N)
  );

  auto start = std::chrono::system_clock::now();
  for(long i = 1; i < N; ++i){
#pragma omp parallel for num_threads(8)
    for(long j = i + 1; j < N; ++j){
      vec[i * i +  j * j].insert({i, j});
    }
  }
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() << "ms\n";

  std::ofstream out("/home/anton/programming/inf_mipt/seventh_sem/mipt_parprog/a2b2n2/out.txt");
  for(long i = 0; i < 2 * N * N; ++i){
    if(vec[i].size() > 1){
      out << i << " ";
      for(auto p: vec[i]){
        out << "(" << p.first << ", " << p.second << ")";
      }
      out << "\n";
    }
  }
}