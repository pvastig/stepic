#include "meta.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <limits>
#include <tuple>

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
typename std::result_of_t<F(Args...)> applyImpl(F&& f, std::tuple<Args...>&& t,
                                                IntList<Indices...>) {
  return std::forward<F>(f)(
      std::get<Indices>(std::forward<std::tuple<Args...>>(t))...);
}

template <typename F, typename... Args, int N = sizeof...(Args)>
typename std::result_of_t<F(Args...)> apply(F&& f, std::tuple<Args...>&& t) {
  return applyImpl(std::forward<F>(f), std::forward<std::tuple<Args...>>(t),
                   typename LogarithmicGenerate::Generate<N>::type());
}

template <typename L1, typename L2, template <int, int> class MetaFun>
struct Zip {
  using type = typename IntCons<
      MetaFun<L1::Head, L2::Head>::value,
      typename Zip<typename L1::Tail, typename L2::Tail, MetaFun>::type>::type;
};

template <template <int, int> class MetaFun>
struct Zip<IntList<>, IntList<>, MetaFun> {
  using type = IntList<>;
};
template <int m = 0, int kg = 0, int s = 0, int A = 0, int K = 0, int mol = 0,
          int cd = 0>
using Dimension = IntList<m, kg, s, A, K, mol, cd>;

// бинарная метафункция
template <int a, int b>
struct Plus {
  static int const value = a + b;
};

template <int a, int b>
struct Minus {
  static int const value = a - b;
};

template <class Type>
class Quantity {
 public:
  Quantity() {}
  explicit Quantity(double value) : m_value(value) {}
  template <class OtherType>
  Quantity(Quantity<OtherType> const& list) : m_value(list.value()) {}
  double value() const { return m_value; }

 private:
  Type m_typeList;
  double m_value;
};

template <class OtherType, class Type,
          class RetType = Quantity<typename Zip<OtherType, Type, Plus>::type>>
auto operator+(Quantity<OtherType> const& l, Quantity<Type> const& r) {
  return RetType(l.value() + r.value());
}

template <class Type>
auto operator+(Quantity<Type> const& l, double value) {
  return Quantity<Type>(l.value() + value);
}

template <class Type>
auto operator+(double value, Quantity<Type> const& l) {
  return (l + value);
}

template <class OtherType, class Type,
          class RetType = Quantity<typename Zip<OtherType, Type, Minus>::type>>
auto operator-(Quantity<OtherType> const& l, Quantity<Type> const& r) {
  return RetType(l.value() - r.value());
}

template <class Type>
auto operator-(Quantity<Type> const& l, double value) {
  return Quantity<Type>(l.value() - value);
}

template <class Type, class RetType = Quantity<Type>>
RetType operator-(double value, Quantity<Type> const& l) {
  return RetType(value - l.value());
}

template <class OtherType, class Type,
          class RetType = Quantity<typename Zip<OtherType, Type, Plus>::type>>
auto operator*(Quantity<OtherType> const& l, Quantity<Type> const& r) {
  return RetType(l.value() * r.value());
}

template <class Type>
auto operator*(Quantity<Type> const& l, double value) {
  return Quantity<Type>(l.value() * value);
}

template <class Type>
auto operator*(double value, Quantity<Type> const& l) {
  return Quantity<Type>(value * l.value());
}

template <class OtherType, class Type,
          class RetType = Quantity<typename Zip<OtherType, Type, Minus>::type>>
auto operator/(Quantity<OtherType> const& l, Quantity<Type> const& r) {
  return RetType(l.value() / r.value());
}

template <class Type>
auto operator/(Quantity<Type> const& l, double value) {
  return Quantity<Type>(l.value() / value);
}

template <class Type>
auto operator/(double value, Quantity<Type> const& l) {
  return Quantity<Type>(value / l.value());
}

using NumberQ = Quantity<Dimension<>>;  // число без размерности
using LengthQ = Quantity<Dimension<1>>;           // метры
using MassQ = Quantity<Dimension<0, 1>>;          // килограммы
using TimeQ = Quantity<Dimension<0, 0, 1>>;       // секунды
using VelocityQ = Quantity<Dimension<1, 0, -1>>;  // метры в секунду
using AccelQ =
    Quantity<Dimension<1, 0, -2>>;  // ускорение, метры в секунду в квадрате
using ForceQ = Quantity<Dimension<1, 1, -2>>;  // сила в ньютонах
}  // namespace IntigerList

void testQuantity() {
  using namespace IntigerList;
  using namespace std;
  {
    LengthQ q1{300};
    auto q = q1 + 100;
    assert(q.value() == 400.0);
    q = 100 + q1;
    assert(q.value() == 400.0);
  }
  {
    LengthQ q1{30000};
    LengthQ q2{20000};
    auto q = q1 + q2;
    cout << q.value() << endl;
  }
  {
    LengthQ q1{30000};
    LengthQ q2{20000};
    auto q = q1 - q2;
    cout << q.value() << endl;
  }
  {
    LengthQ q1{30};
    LengthQ q2{20};
    auto q = q1 * q2;
    cout << q.value() << endl;
  }
  {
    LengthQ q1{30};
    LengthQ q2{10};
    auto q = q1 / q2;
    cout << q.value() << endl;
  }
  {
    LengthQ q1{30000};
    NumberQ q2{20000};
    auto q = q1 + q2;
    cout << q.value() << endl;
  }
  {
    LengthQ l{30000};     // 30 км
    TimeQ t{10 * 60};     // 10 минут
                          // вычисление скорости
    VelocityQ v = l / t;  // результат типа VelocityQ, 50 м/с

    AccelQ a{9.8};  // ускорение свободного падения
    MassQ m{80};    // 80 кг
    // сила притяжения, которая действует на тело массой 80 кг
    ForceQ f = m * a;
    cout << f.value() << endl;
  }
  {
    // using Q1 = Quantity<IntList<-20, -18, -16, -14, -12, -10, -8>>;
    int N = 2;
    using Q1 = Quantity<IntList<-10, -9, -8, -7, -6, -5, -4>>;
    Q1 a = Q1(N) + Q1(N * N);
  }
}

void testTrsnsform() {
  using namespace IntigerList;

  // два списка одной длины
  using L1 = IntList<1, 2, 3, 4, 5>;
  using L2 = IntList<1, 3, 7, 7, 2>;

  // результат применения — список с поэлементными суммами
  // using L3 = Zip<L1, L2, Plus>::type;  // IntList<2, 5, 10, 11, 7>
  // print<L3>();
}

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

void testList() {
  using namespace std;
  using namespace IntigerList;
}

void testLength() {
  using namespace std;
  using namespace IntigerList;
  /*using primes = IntList<2, 3, 4>;
  int head = primes::Head;
  using odd_primes = primes::Tail;
  constexpr size_t len = Length<primes>::value;*/
}

void testIntCons() {
  using namespace std;
  using namespace IntigerList;
  using primes = IntList<2, 3, 4>;
  using L2 = IntCons<1, primes>::type;
}

void testGenerate() {
  using namespace std;
  using namespace IntigerList;
  // using L3 = LinearGenerate::Generate<1000>::type;
  // using L3 = LogarithmicGenerate::Generate<1000>::type;
}

void runMetaTest() {
  using namespace std;
  using namespace IntigerList;
  // testApply();
  // testTrsnsform();
  testQuantity();
}
