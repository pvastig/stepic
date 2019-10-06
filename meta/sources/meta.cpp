#include "meta.h"

#include <iostream>
#include <limits>

namespace Fibonacci {
template <int N>
struct Fib {
  static int const value = Fib<N - 1>::value + Fib<N - 2>::value;
};

template <>
struct Fib<0> {
  static int const value = 0;
};

template <>
struct Fib<1> {
  static int const value = 1;
};
}  // namespace Fibonacci

namespace IntigerList {

template <int... Types>
struct IntList;

template <>
struct IntList<> {};

template <int H, int... T>
struct IntList<H, T...> {
  static const int Head = H;
  using Tail = IntList<T...>;
};

template <class TL>
struct Length {
  static int const value = 1 + Length<typename TL::Tail>::value;
};

template <>
struct Length<IntList<>> {
  static int const value = 0;
};

template <int H, class... TL>
struct IntCons;

template <int H, int... T>
struct IntCons<H, IntList<T...>> {
  using type = IntList<H, T...>;
};

template <typename, typename>
struct ConcatLists;

template <int... S1, int... S2>
struct ConcatLists<IntList<S1...>, IntList<S2...>> {
  using type = IntList<S1..., (sizeof...(S1) + S2)...>;
};

namespace LinearGenerate {
template <int N, int M = 0>
struct Generate {
  using type = typename IntCons<M, typename Generate<N - 1, M + 1>::type>::type;
};

template <int M>
struct Generate<0, M> {
  using type = IntList<>;
};
}  // namespace LinearGenerate

namespace LogarithmicGenerate {
template <int N>
struct Generate;

template <>
struct Generate<1> {
  using type = IntList<0>;
};

template <int N>
struct Generate {
  using type =
      typename ConcatLists<typename Generate<(N >> 1)>::type,
                           typename Generate<N - (N >> 1)>::type>::type;
};

}  // namespace LogarithmicGenerate
template <typename IL>
void print() {
  std::cout << IL::Head << " ";
  print<typename IL::Tail>();
};

template <>
void print<IntList<>>() {
  std::cout << std::endl;
};

}  // namespace IntigerList

void runMetaTest() {
  using namespace std;
  using namespace IntigerList;
  /*using primes = IntList<2, 3, 4>;
  int head = primes::Head;
  using odd_primes = primes::Tail;
  constexpr size_t len = Length<primes>::value;
  using L2 = IntCons<1, primes>::type;*/

  // using L3 = LinearGenerate::Generate<1000>::type;
  // using L3 = LogarithmicGenerate::Generate<1000>::type;
  // print<L3>();
  // cout << Length<L3>::value << endl;
}
