#include <iostream>
#include <omp.h>
#include <string>
#include <random>
#include <chrono>
#include <cassert>
#include <fstream>
#include "Matrix.hpp"

long NUM_THREADS = 8;
long SIZE = 7;
bool ALL = false;
long ITER = 21;


bool parse_args(int argc, char *argv[]){
  for(int i = 1; i < argc; ++i)
  {
    if(std::string("--all") == std::string(argv[i])) {
      ALL = true;
    }else if (std::string("--size") == std::string(argv[i])){
      ++i;
      if(i >= argc)
        return false;
      SIZE = strtol(argv[i], nullptr, 10);
    }else if (std::string("--num_treads") == std::string(argv[i])){
      ++i;
      if(i >= argc)
        return false;
      NUM_THREADS  = strtol(argv[i], nullptr, 10);
    }
  }
  return true;
}

const char* PATH = "/home/anton/programming/inf_mipt/seventh_sem/mipt_parprog/results/res1.txt";


int main(int argc, char *argv[]){
  if(!parse_args(argc, argv)){
    std::cout << "correct input format is:\n\t./matrix_baseline [--all] [--size n] [--num_threads m]\n";
    return 0;
  }
  omp_set_num_threads(static_cast<int>(NUM_THREADS));
  std::default_random_engine gen;
  std::uniform_real_distribution<double> dis(-1, 1);
  std::ofstream out(PATH);
  long l = SIZE, r = SIZE + 1;
  if (ALL){
    l = 10;
    r = 2000;
  }
  for(long size = l; size < r; ++size) {
    Matrix A(size, size);
    Matrix B(size, size);
    if(!(size % 10))
      std::cout << size <<"\n";
    out << size <<" ";
    for(long cnt = 0; cnt < ITER; ++cnt) {

      for (int i = 0; i < size; ++i) {
        auto line1 = A[i];
        auto line2 = B[i];
        for (int j = 0; j < size; ++j) {
          line1[j] = dis(gen);
          line2[j] = dis(gen);
        }
      }
      auto start = std::chrono::high_resolution_clock::now();
      Matrix C = naive_matmul(A, B);
      out << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() << "\t";
      start = std::chrono::high_resolution_clock::now();
      Matrix D = naive_matmul_parallel(A, B);
      out << -std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() << "\t";
      assert(D == C);
    }
    out << "\n";
  }
  return 0;
}