#include <iostream>

void foo(double n) { std::cout << "foo(double)" << std::endl; }

void foo(int n) { std::cout << "foo(int)" << std::endl; }

int main() {
  foo(1.0);
  foo(1); // Implicit conversion of int to double without loss of percision
}