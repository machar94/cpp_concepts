#include <iostream>
#include <assert.h>
#include <vector>

template <typename Iter>
typename Iter::value_type sum(Iter b, Iter e, typename Iter::value_type acc)
{
    std::cout << "sum(Iter b, Iter e, typename Iter::value_type)" << std::endl;

    while (b != e)
    {
        acc += *b++;
    }
    return acc;
}

template <typename T>
T sum(T *b, T *e, T acc)
{
    std::cout << "sum(T* b, T* e, Tacc)" << std::endl;
    while (b != e)
    {
        acc += *b++;
    }
    return acc;
}

int sum(int *b, int *e, int acc)
{
    std::cout << "sum(int* b, int* e, int acc)" << std::endl;
    while (b != e)
    {
        acc += *b++;
    }
    return acc;
}

int main()
{
    std::vector<int> a{1, 2, 3, 4};
    std::vector<double> b{1, 2, 3, 4};
    auto res0 = sum(a.begin(), a.end(), 0);
    auto res1 = sum(a.data(), a.data() + 4, 0);
    auto res2 = sum(b.data(), b.data() + 4, 0.0);
    
    assert(res0 == 10);
    assert(res1 == 10);
    assert(res2 == 10);
}

// 2. With funciton templates (it gets a bit more complicated)
// Now compiler also considers function templates in overload resolution
// function template is not a funciton on its own, only when we provide type
// perfers normal functions over function template specialization
// if no viable funciton is found, then it's an error

// 3. For res0, it tried to first substitute in the Iter templated version and passed
// but it didn't stop there.. it then tried to substitue in the T* version of sum and failed
// but IT DID NOT RAISE AN ERROR
// for res1, same as before Int* doesn't have a ::value_type, so it fails on sum #1,
// but succeeds on sum #2.

// 4. res0 Failed twice but succeeded on Typename Iter function
// res1 fails only on Typename Iter, succeeds on both the others, but prefers the regular funciton that matches exactly
// res2 fails on Typename Iter, succeeds on the template, and then doesn't even consider int* because it takes int
// sum(Iter b, Iter e, typename Iter::value_type)
// sum(int* b, int* e, int acc)
// sum(T* b, T* e, Tacc)

// We can use it by stating that we don't want a certain function template specialization to be 
// used in overload resolution. In our case we excluded all types that don't have type member
// ::value_type
// .. in C++ 20, this idea was replaced by concepts

// Main takeaway of SFINAE:
// * Don't worry if of all the av