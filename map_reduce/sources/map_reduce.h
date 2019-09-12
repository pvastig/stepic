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
    {
        std::list<int> l = {1,2,3,4,5,6,7,8,9,10};
        for (size_t i = 1; i <= 10; ++i)
        {
            auto sum = map_reduce(l.begin(), l.end(),
                [](int i){return i;},
                std::plus<int>(), i);
            assert(sum == 55);

            auto has_even = map_reduce(l.begin(), l.end(),
                [](int i){return i % 2 == 0;},
                std::logical_or<bool>(), i);
            assert(has_even == 1);
        }
    }

    {
        std::list<int> l = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        for (size_t i = 1; i <= 10; ++i)
        {
            auto sum = map_reduce(l.begin(), l.end(),
                [](int i){return i;},
                std::plus<int>(), i);
            assert(sum == 10);

            auto has_even = map_reduce(l.begin(), l.end(),
                [](int i){return i % 2 == 0;},
                std::logical_or<bool>(), i);
            assert(has_even == 0);
        }
    }
}
