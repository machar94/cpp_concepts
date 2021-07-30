#include <algorithm>
#include <future>
#include <iostream>
#include <random>
#include <set>

// Debug
// /usr/bin/time ./install/threading/02_future
// 0.96user 0.01system 0:00.98elapsed 99%CPU (0avgtext+0avgdata
// 33088maxresident)k 0inputs+0outputs (0major+7540minor)pagefaults 0swaps

// Release
// /usr/bin/time ./install/threading/02_future
// 0.49user 0.01system 0:00.50elapsed 99%CPU (0avgtext+0avgdata
// 33004maxresident)k 0inputs+0outputs (0major+7552minor)pagefaults 0swaps

// Debug (two threads)
// 2.96user 0.03system 0:02.05elapsed 145%CPU (0avgtext+0avgdata
// 92744maxresident)k 0inputs+0outputs (0major+22389minor)pagefaults 0swaps

std::set<int> make_sorted_random(const size_t num_elems) {
  std::set<int> retval;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, num_elems - 1);

  // inserter is a convenience function that generates a std::insert_iterator
  // which takes (1) container and (2) the location to insert. generate_n
  // takes an insert_iterator and a lambda
  std::generate_n(std::inserter(retval, retval.end()), num_elems,
                  [&]() { return dis(gen); });

  // throw std::runtime_error("Hello World!");

  return retval;
}

int main() {

  // Async
  // 2.96user 0.04system 0:02.06elapsed 145%CPU (0avgtext+0avgdata
  // 92740maxresident)k 0inputs+0outputs (0major+22381minor)pagefaults 0swaps
  //   auto f1 = std::async(std::launch::async, make_sorted_random, 1000000);
  //   auto f2 = std::async(std::launch::async, make_sorted_random, 1000000);

  // Deferred
  // 2.03user 0.03system 0:02.06elapsed 100%CPU (0avgtext+0avgdata
  // 62860maxresident)k 0inputs+0outputs (0major+14956minor)pagefaults 0swaps
  auto f1 = std::async(std::launch::deferred, make_sorted_random, 1000000);
  auto f2 = std::async(std::launch::deferred, make_sorted_random, 1000000);

  //   try {
  //     f1.get();
  //   } catch (const std::exception &e) {
  //     std::cout << "f1 threw exception: " << e.what() << "\n";
  //   }

  std::cout << f1.get().size() << " " << f2.get().size() << "\n";
}