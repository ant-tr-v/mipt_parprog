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
long ITER = 7;


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

const char* PATH = "/home/anton/programming/inf_mipt/seventh_sem/mipt_parprog/Matrix/results/res2.txt";


int main(int argc, char *argv[]){
  if(!parse_args(argc, argv)){
    std::cout << "correct input format is:\n\t./matrix_baseline [--all] [--size n] [--num_threads m]\n";
    return 0;
  }
  omp_set_num_threads(static_cast<int>(NUM_THREADS));
  std::default_random_engine gen(time(nullptr));
  std::uniform_real_distribution<double> dis(-1, 1);
  std::ofstream out(PATH);
  long l = SIZE, r = SIZE + 1;
  if (ALL){
    l = 2;
    r = 1024;
  }
  std::vector<long> num;
  std::vector<std::vector<long> > times_naive(static_cast<unsigned long>(ITER));
  std::vector<std::vector<long> > times_naive_parallel(static_cast<unsigned long>(ITER));
  std::vector<std::vector<long> > times_transposed(static_cast<unsigned long>(ITER));
  std::vector<std::vector<long> > times_transposed_parallel(static_cast<unsigned long>(ITER));
  //std::vector<std::vector<long> > times_strassen(static_cast<unsigned long>(ITER));
  std::vector<std::vector<long> > times_strassen_parallel(static_cast<unsigned long>(ITER));

  for(long cnt = 0; cnt < ITER; ++cnt) {
    for (long size = l; size <= r; size *= 2) {
      Matrix A(size, size);
      Matrix B(size, size);
      std::cout << cnt + 1 <<"|" <<size << "\n";
      if(cnt == 0)
        num.push_back(size);
      for (int i = 0; i < size; ++i) {
        auto line1 = A[i];
        auto line2 = B[i];
        for (int j = 0; j < size; ++j) {
          line1[j] = dis(gen);
          line2[j] = dis(gen);
        }
      }
      //omp_set_num_threads(static_cast<int>(NUM_THREADS));
      auto start = std::chrono::high_resolution_clock::now();
      Matrix C = naive_matmul(A, B);
      times_naive[cnt].push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());
      start = std::chrono::high_resolution_clock::now();
      Matrix D = naive_matmul_parallel(A, B);
      times_naive_parallel[cnt].push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());
      start = std::chrono::high_resolution_clock::now();
      Matrix E = transposed_matmul(A, B);
      times_transposed[cnt].push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());
      start = std::chrono::high_resolution_clock::now();
      Matrix F = transposed_matmul_parallel(A, B);
      times_transposed_parallel[cnt].push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());
      start = std::chrono::high_resolution_clock::now();
//      Matrix G = strassen_matmul(A, B);
//      times_strassen[cnt].push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());
//      start = std::chrono::high_resolution_clock::now();
      Matrix H = strassen_matmul_parallel(A, B);
      times_strassen_parallel[cnt].push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());

      assert(D == C);
      assert(E == C);
      assert(F == C);
      //assert(G == C);
      assert(H == C);
    }
  }


  for (int i = 0; i < num.size(); ++i) {
    out << "naive" << " " << num[i];
    for (long cnt = 0; cnt < ITER; ++cnt) {
      out << " " << times_naive[cnt][i];
    }
    out <<"\n";
    out << "naive_parallel" << " " << num[i];
    for (long cnt = 0; cnt < ITER; ++cnt) {
      out << " " << times_naive_parallel[cnt][i];
    }
    out <<"\n";
    out << "transposed" << " " << num[i];
    for (long cnt = 0; cnt < ITER; ++cnt) {
      out << " " << times_transposed[cnt][i];
    }
    out <<"\n";
    out << "transposed_parallel" << " " << num[i];
    for (long cnt = 0; cnt < ITER; ++cnt) {
      out << " " << times_transposed_parallel[cnt][i];
    }
    out <<"\n";
//    out << "strassen" << " " << num[i];
//    for (long cnt = 0; cnt < ITER; ++cnt) {
//      out << " " << times_strassen[cnt][i];
//    }
//    out <<"\n";
    out << "strassen_parallel" << " " << num[i];
    for (long cnt = 0; cnt < ITER; ++cnt) {
      out << " " << times_strassen_parallel[cnt][i];
    }
    out <<"\n";
  }
  return 0;
}