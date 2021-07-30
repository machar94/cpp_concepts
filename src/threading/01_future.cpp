#include <atomic>
#include <future>
#include <iostream>
#include <random>
#include <thread>

void work(int i) {
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<unsigned int> rnd(333, 777);
  // simulate heavy work...
  auto slept_for = std::chrono::milliseconds(rnd(rng));
  std::this_thread::sleep_for(slept_for);
  std::cout << "Future " << i << " slept for "
            << std::to_string(slept_for.count()) << std::endl;
}
int main() {
  std::cout << "Main thread ID: " << std::this_thread::get_id() << std::endl;

  // (1) std::launch::async
  // (2) std::launch::deferred
  // The launch policy
  // (1) launches a new thread at some point
  // (2) lazy launches a new thread (only starts thread when get is called)

  // Type: std::future<void>
  auto f1 = std::async(std::launch::async, work, 1);
  auto f2 = std::async(std::launch::async, work, 2);

  //   std::atomic<int> ret;
  //   auto t1 = std::thread([&ret]() {
  //     std::cout << "Explicit thread ID: " << std::this_thread::get_id()
  //               << std::endl;
  //     ret = 1;
  //   });

  std::cout << "Calling f1.get()" << std::endl;
  f1.get();
  std::cout << "Calling f2.get()" << std::endl;
  f2.get();

  // t1.join();
  // std::cout << "Return value from explicit thread: " << ret << std::endl;
}

// The future async example reminds me of thread and join. Except you needed to
// explicitly pass in the variable by reference that you wanted to update.
//  Important, once you calll get(), you cannot grab a value from it again()!

// Get is blocking, meaning that the code above doesn't work
// If it's a thread that's supposed to end before the program then maybe async
// is better than thread.

// get, waits for the task to finish and get the value. It is blocking but if
// the other threads have already spawned and in particular other asyncs with
// the async launch policy, then they will still continue to do work.
// wait_for
// valid: is false after a .get() 
// wait_until 
// future_status::ready 
// async essentially a thread but you know that it will return before the program ends. Returns a future
// future holds the value of something returning from an async call. Future values don't hold a real value until the async function ends.
// promise

// Takeaway from Jason Turner
// Really easy way to start getting parallelization benefits if you stick with
// copys of data and no globals (that way no locking is required)

// default value of async is async | deferred, meaning the computer can decide
// if it wants to launch the task in a new thread or run it synchronously when
// get is called
