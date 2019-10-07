#include "meta.h"

#include <functional>
#include <iostream>
#include <limits>
#include <tuple>
#include <type_traits>

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

template <class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch, Tr>& os, const Tuple& t,
                      std::index_sequence<Is...>) {
  ((os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), ...);
}

template <class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
  os << "(";
  print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
  return os << ")";
}

template <typename F, typename... Args, int... Indices>
auto applyImpl(F&& f, std::tuple<Args...>&& t, IntList<Indices...>)
    -> decltype(std::forward<F>(f)(
        std::get<Indices>(std::forward<std::tuple<Args...>>(t))...)) {
  return std::forward<F>(f)(
      std::get<Indices>(std::forward<std::tuple<Args...>>(t))...);
}

template <typename F, typename... Args, int N = sizeof...(Args)>
auto apply(F&& f, std::tuple<Args...>&& t)
    -> decltype(applyImpl(std::forward<F>(f),
                          std::forward<std::tuple<Args...>>(t),
                          typename LogarithmicGenerate::Generate<N>::type())) {
  return applyImpl(std::forward<F>(f), std::forward<std::tuple<Args...>>(t),
                   typename LogarithmicGenerate::Generate<N>::type());
}
}  // namespace IntigerList

void testApply() {
  {
    auto f = [](int x, double y, double z) { return x + y + z; };
    auto t = std::make_tuple(1.0, 2.0, 3);  // std::tuple<int, double, double>
    std::cout << apply(f, t) << std::endl;
  }

  {
    auto f = [](bool x, bool y) { return x == y; };
    auto t = std::make_tuple(true, true);  // std::tuple<int, double, double>
    std::cout << apply(f, t) << std::endl;
  }
  {
    auto f = [](std::string x, std::string y) { return x + y; };
    auto t = std::make_tuple("Hello", " world");
    std::cout << apply(f, t) << std::endl;
  }

  {
    std::tuple<int, int> t;
    auto f = [](int x, int y) { return x + y; };
    std::cout << apply(f, t) << std::endl;
  }

  {
    std::tuple<> t;
    auto f = []() { return; };
    apply(f, t);
  }

  {
    std::tuple<const char*> t{"test char"};
    auto f = [](const char* ch) { return ch; };
    std::cout << apply(f, t) << std::endl;
  }

  {
    std::tuple<int, double, const char*, int, double, const char*> t{
        1, 1.2, "test", 0, 5.0, "world"};
    auto f = [](int, double, const char*, int, double, const char*) {
      return 1;
    };
    std::cout << apply(f, t) << std::endl;
  }
  {
    std::tuple<int, double, std::string> t{1, 2.0, "t"};
    auto f = [](int x2, double y2, std::string s2) {
      return x2 * y2 + s2.size();
    };
    std::cout << apply(f, t) << std::endl;
  }
}
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

  testApply();
}
