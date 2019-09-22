#include <algorithm>
#include <cassert>
#include <future>
#include <thread>
#include <iostream>
#include <list>
#include <vector>
#include <numeric>

template<class Iterator, class UnaryOp, class BinaryOp>
auto map_reduce(Iterator p, Iterator q, UnaryOp f1, BinaryOp f2, size_t numThreads)->decltype(f1(*p))
{
    auto calcPartSum = [f1, f2](Iterator p, Iterator q)
    {
        auto res = f1(*p);
        while (++p != q)
            res = f2(res, f1(*p));
        return res;
    };

    using ResType  = decltype(f1(*p));
    auto const len = std::distance(p, q);
    if (!len || len < 0)
        return ResType{};

    if (numThreads > static_cast<size_t>(len))
        return ResType{};

    auto const blockSize = static_cast<size_t>(len) / numThreads;
    std::vector<std::future<ResType>> threads(numThreads-1);
    auto blockStart = p;
    for (size_t i = 0; i < (numThreads-1); ++i)
    {
        auto blockEnd = blockStart;
        advance(blockEnd, blockSize);
        threads[i] = std::async(std::launch::async, calcPartSum, blockStart, blockEnd);
        blockStart = blockEnd;
    }

    std::vector<ResType> results(numThreads);
    results.reserve(numThreads);
    auto res = calcPartSum(blockStart, q);
    for (auto && thread : threads)
        results.push_back(thread.get());
    results.push_back(res);

    return std::accumulate(results.begin(), results.end(), ResType(),
                           [f2](ResType l, ResType r) { return  f2(l, r); });
}

template<class It, class UnaryOp, class BinaryOp>
auto map_reduce_one_thread(It p, It q, UnaryOp f1, BinaryOp f2)
{
    auto res = f1(*p);
    while(++p != q)
        res = f2(res, f1(*p));
    return res;
}

template<class Iterator, class UnaryOp, class BinaryOp>
auto map_reduce2(Iterator p, Iterator q, UnaryOp f1, BinaryOp f2, size_t numThreads)->decltype(f1(*p))
{
    auto calcPartSum = [f1, f2](Iterator p, Iterator q)
    {
        auto res = f1(*p);
        while (++p != q)
            res = f2(res, f1(*p));
        return res;
    };

    using ResType  = decltype(f1(*p));
    auto const len = std::distance(p, q);
    if (!len || len < 0)
        return ResType{};

    if (numThreads > static_cast<size_t>(len))
        return ResType{};

    std::vector<ResType> results(numThreads);
    auto const blockSize = static_cast<size_t>(len) / numThreads;
    std::vector<std::thread> threads(numThreads-1);
    auto blockStart = p;
    for (size_t i = 0; i < (numThreads-1); ++i)
    {
        auto blockEnd = blockStart;
        advance(blockEnd, blockSize);
        threads[i] = std::thread([&results, i, calcPartSum, blockStart, blockEnd]()
                                 { results[i] = calcPartSum(blockStart, blockEnd); });
        blockStart = blockEnd;
    }

    results[numThreads-1] = calcPartSum(blockStart, q);
    for (auto & thread : threads)
            thread.join();

    return std::accumulate(results.begin(), results.end(), ResType(),
                           [f2](ResType l, ResType r) { return  f2(l, r); });
}

void runMapReduce()
{
    {
        std::list<int> l = {1,2,3,4,5,6,7,8,9,10};
        for (size_t i = 1; i <= 10; ++i)
        {
            auto sum = map_reduce2(l.begin(), l.end(),
                [](int i){return i;},
                std::plus<int>(), i);
            std::cout << sum << std::endl;
            assert(sum == 55);

            auto has_even = map_reduce2(l.begin(), l.end(),
                [](int i){return i % 2 == 0;},
                std::logical_or<bool>(), i);
            assert(has_even == 1);
        }
    }

    {
        std::list<int> l = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        for (size_t i = 1; i <= 10; ++i)
        {
            auto sum = map_reduce2(l.begin(), l.end(),
                [](int i){return i;},
                std::plus<int>(), i);
            assert(sum == 10);

            auto has_even = map_reduce2(l.begin(), l.end(),
                [](int i){return i % 2 == 0;},
                std::logical_or<bool>(), i);
            assert(has_even == 0);
        }
    }
}
