#include <future>
#include <iostream>
#include <thread>

int main() {
  std::cout << "Main thread ID: " << std::this_thread::get_id() << std::endl;

  auto future = std::async(std::launch::async, []() {
    std::cout << "Async thread ID: " << std::this_thread::get_id() << std::endl;
  });

  future.get();
}