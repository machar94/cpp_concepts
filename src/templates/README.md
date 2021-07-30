# SFINAE

Substitution Failure Is Not An Error (SFINAE). 

It is related to function overloading (many functions of the same name but with
different argument types). Each function is called an overloaded function.
Overload resolution is the name of the process by which the compiler chooses the
best function out of the set of available overloaded functions based on the
argument types provided.

If there are function templates also available (templates with the same funciton
name), then overload resolution will also consider them. It however, prefers non
templated functions over templated ones. If during the overload resolution
process, the compiler tries to substitute the type in a template and fails, the
compiler just moves on and tests the next function. If the function that failed
is a template, then this process of moving on to the next one is because
substitution failure is not an error (SFINAE).

**Takeaways** 

The compiler will try to match against all available function/class templates
and not error out if a substitution fails.


## Example 1

This is just an example of function overloading. There are two `foo` functions
to choose from. If `foo(int n)` is commented out, the compiler will perform an
implicit conversion on 1 with no loss of precision and call `foo(double n)`.
There are no compiler errors. If `foo(int n)` is available, then for `foo(1)`
the compiler chooses it since the function prototype is an exact match. There is
no SFINAE here. 

```
#include <iostream>

void foo(double n)
{
    std::cout << "foo(double)" << std::endl;
}

/*
void foo(int n)
{
    std::cout << "foo(int)" << std::endl;
}
*/

int main()
{
    foo(1.0);
    foo(1); // Implicit conversion of int to double without loss of percision
}
```

## Example 2

Let start with just this function. Any type Iter, should also have a member
`::value_type`.

```
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
```

In the below code, evaluating the first sum call succeeds... the iterators of a
vector have `::value_type` defined, but it isn't defined for `int*` which is why
the second call to sum fails. Yes, this is an error, but it is an error because
there were no other overloaded functions to match with.

```
int main()
    std::vector<int> a{1, 2, 3, 4};
    auto res0 = sum(a.begin(), a.end(), 0);
    auto res1 = sum(a.data(), a.data() + 4, 0);
}
```

The error you might receive may look like this:
```
error: no matching function for call to ‘sum(int*, int*, int)’
    ...
    template argument deduction/substitution failed:
```

If we now add this overload of sum below then everything works. Now in the above
code, both calls to sum succeed. Specifically in the case of the second call,
the compiler will try and substitute `Iter` as `int*` and fail, but will then
move on to the next template which expects a pointer and will succeed (SFINAE).

```
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
```

Finally consider a third implementation of sum.

```
int sum(int *b, int *e, int acc)
{
    std::cout << "sum(int* b, int* e, int acc)" << std::endl;
    while (b != e)
    {
        acc += *b++;
    }
    return acc;
}
```

Main
```
int main()
{
    std::vector<int> a{1, 2, 3, 4};
    std::vector<double> b{1, 2, 3, 4};
    auto res0 = sum(a.begin(), a.end(), 0);
    auto res1 = sum(a.data(), a.data() + 4, 0);
    auto res2 = sum(b.data(), b.data() + 4, 0.0);
}
```

Output
```
sum(Iter b, Iter e, typename Iter::value_type)
sum(int* b, int* e, int acc)
sum(T* b, T* e, Tacc)
```

When we run the code:
* Call 1 will fail twice on option T* because it is not a pointer and on option
  int* because the compiler won't convert an interator to an int*
* Call 2 will fail once on the template expecting type `Iter` as specified
  previously but will succeed on sum calls two and three. Note the output of running the code actually shows the int* implementation was called because the compiler was able to (and prefers) matching with a non templated function over a templated option.
* Call 3 will fail twice, but succeeds on the T* option of sum since the
  compiler can substite T for double and then match the call exactly. 