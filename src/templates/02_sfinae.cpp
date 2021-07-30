#include <assert.h>

#include <iostream>
#include <vector>

template <typename Iter>
typename Iter::value_type sum(Iter b, Iter e, typename Iter::value_type acc) {
  std::cout << "sum(Iter b, Iter e, typename Iter::value_type)" << std::endl;

  while (b != e) {
    acc += *b++;
  }
  return acc;
}

template <typename T> T sum(T *b, T *e, T acc) {
  std::cout << "sum(T* b, T* e, Tacc)" << std::endl;
  while (b != e) {
    acc += *b++;
  }
  return acc;
}

int sum(int *b, int *e, int acc) {
  std::cout << "sum(int* b, int* e, int acc)" << std::endl;
  while (b != e) {
    acc += *b++;
  }
  return acc;
}

int main() {
  std::vector<int> a{1, 2, 3, 4};
  std::vector<double> b{1, 2, 3, 4};
  auto res0 = sum(a.begin(), a.end(), 0);
  auto res1 = sum(a.data(), a.data() + 4, 0);
  auto res2 = sum(b.data(), b.data() + 4, 0.0);

  assert(res0 == 10);
  assert(res1 == 10);
  assert(res2 == 10);
}