#include <future>
#include <iostream>
#include <list>
#include <vector>
#include <cassert>

using namespace std;

template<class It, class UnaryOp, class BinaryOp>
auto map_reduce(It p, It q, UnaryOp f1, BinaryOp f2, size_t threads)->decltype(f1(*p))
{
    auto calcPartSum = [f1, f2](It it, size_t dist)
    {
        auto res = f1(*it);
        for (decltype(dist) i = 1; i < dist; ++i)
            res = f2(res, f1(*++it));
        return res;
    };

    std::vector<std::future<decltype(f1(*p))>> result;
    result.reserve(threads);
    auto const len  = std::distance(p, q);
    auto const r    = static_cast<size_t>(len) % threads;
    auto const step = static_cast<size_t>(len) / threads;
    result.push_back(std::async(std::launch::async, calcPartSum, p, step+r));
    advance(p, step + r);
    for (size_t i = 1; i < threads; ++i)
    {
        result.push_back(std::async(std::launch::async, calcPartSum, p, step));
        advance(p, step);
    }

    auto res = result.begin()->get();
    for (auto it = std::next(result.begin()); it != result.end(); ++it)
        res = f2(res, it->get());

    return res;
}

template<class It, class UnaryOp, class BinaryOp>
auto map_reduce2(It p, It q, UnaryOp f1, BinaryOp f2)
{
    auto res = f1(*p);
    while(++p != q)
    {
        res = f2(res, f1(*p));
    }
    return res;
}

void runMapReduce()
{
    std::list<int> l = {1,2,3,4,5,6,7,8,9,10};
    //std::list<int> l = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    //std::list<int> l = {1,1,1};
    //std::list<string> l = {"1", "2", "3","4", "5","6", "7", "8", "9"};
    // параллельное суммирование в 3 потока
    /*auto sum = map_reduce(l.begin(), l.end(),
        [](int i){return i;},
        std::plus<int>(), 10);*/
    auto sum = map_reduce(l.begin(), l.end(),
        [](int i){return i;},
        [](int n1, int n2){return n1*n2;}, 2);
    cout << sum << endl;

    /*auto sum1 = map_reduce2(l.begin(), l.end(),
        [](int i){return i;},
        std::plus<int>());*/
    auto sum1 = map_reduce2(l.begin(), l.end(),
        [](int i){return i;},
        [](int n1, int n2){return n1*n2;});
    cout << "real: " << sum1 << endl;

    assert(sum == sum1);

    // проверка наличия чётных чисел в четыре потока
    /*auto has_even = map_reduce(l.begin(), l.end(),
        [](int i){return i % 2 == 0;},
        std::logical_or<bool>(), 4);
    cout << has_even << endl;

    auto has_even2 = map_reduce2(l.begin(), l.end(),
        [](int i){return i % 2 == 0;},
        std::logical_or<bool>());
    cout << "real: " << has_even2 << endl;*/
}
