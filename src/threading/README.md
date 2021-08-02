# Threading

* Mutexes/Locks
* Condition Variables
* Async/Future
* Promise
* Thread pools

Concepts about Async, Future, and Promise are modern ways (C++11) of using
concurrency.

## Async/Future

Take a look below at `01_future.cpp`. Our 'work' function takes one input N
which is used to generate a (programming definition) set of &le; N sorted
elements drawn from a uniform integer distribution from [0, N-1].
```
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

  return retval;
}
```

### Example 1
After building and running the function with `time`, we see that it takes about
two seconds to run sequentially.

```
# cd root
$ cmake -S . -B build
$ cmake --build build
$ cmake --install build --prefix install
$ /usr/bin/time ./install/threading/01_future.cpp
631951, 631423
1.87user 0.02system 0:01.90elapsed 99%CPU (0avgtext+0avgdata 62596maxresident)k
0inputs+0outputs (0major+14940minor)pagefaults 0swaps
```

### Example 2

Now let's start using async and future. Use async when you want to spawn a
thread to do some work that should finish much earlier than the program (as
opposed to threads that are expected to live for the lifetime of the program
handling requests). Futures are used to capture the return value of an async
thread. Futures are templated and take the form of `std::future<Type>` (e.g.
`std::future<int>` for `int` return values or `std::future<void>` for void
functions). Future objects have a `get()` method which is a blocking call. If
the result of an async is not ready yet, the current thread will stop and wait
until the async function finishes. Calling `get()` on a future object that
already has had its value read results in an error. To check if a future object has
a valid return value stored, you can optionally check the `valid()`.

```
// Ex. 2 Async - std::launch::async | std::launch::deferred
auto f1 = std::async(make_sorted_random, 1000000);
auto f2 = std::async(make_sorted_random, 1000000);
std::cout << f1.get().size() << " " << f2.get().size() << "\n";
```

Time
```
2.09user 0.03system 0:01.14elapsed 185%CPU (0avgtext+0avgdata 62984maxresident)k
0inputs+0outputs (0major+14962minor)pagefaults 0swaps
```

The computer took 185% of the CPU which indicates that the computer was able to
run the two tasks in parallel. `async` takes 2+ arguments, the first of which is
a launch policy. `std::launch::async` specifies to run the async function in
it's own thread. `std::launch::deferred` specifies to evaluate the function
lazily. In deferred mode, only when the `get()` method is called will the async
task be run. If the async function does not return a value, the `get()` method
can be called on an object of type `std::future<void>`. If no policy is provided
the policy is set to `std::launch::async | std::launch::deferred` (bit flags)
which means leave the choice up to the implementation. To make sure a thread is
spawned for the async task, set the policy to async.

Time (deferred)
```
1.95user 0.01system 0:01.96elapsed 99%CPU (0avgtext+0avgdata 62952maxresident)k
0inputs+0outputs (0major+14955minor)pagefaults 0swaps
```

Notice that when using the deferred policy, the program runs essentially
sequentially. `f1.get()` runs the first async task and returns. Then `f2.get()`
runs the second async task.

### Example 3

Using futures also makes it really easy to handle exceptions. Just wrap the `get()` method in a try catch block like so:

```
try {
  f1.get();
} catch (const std::exception &e) {
  std::cout << "f1 threw exception: " << e.what() << "\n";
}
```

### Note

The future/get combo is pretty similar in usage to thread/join.