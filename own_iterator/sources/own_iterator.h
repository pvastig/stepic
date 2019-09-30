#include <list>
#include <vector>
#include <cassert>
#include <iostream>

template<class T>
class VectorList
{
private:
    using VectT  = std::vector<T>;
    using ListT  = std::list<VectT>;
    ListT data_;

public:
    using value_type = T;

    VectorList() = default;
    VectorList(VectorList const &) = default;
    VectorList(VectorList &&) = default;

    VectorList & operator=(VectorList &&)     = default;
    VectorList & operator=(VectorList const &) = default;

    template<class It>
    void append(It p, It q)
    {
        if (p != q)
            data_.push_back(VectT(p,q));
    }

    bool empty() const { return size() == 0; }

    size_t size() const
    {
        size_t s = 0;
        for (auto const & item : data_)
            s += item.size();
        return s;
    }

    struct BidirectionalIterator : std::iterator<std::bidirectional_iterator_tag,
                                                 T const>
    {
        ListT const * data = nullptr;
        using ConstListIt = typename ListT::const_iterator;
        using ConstVecIt = typename VectT::const_iterator;
        ConstListIt clit;
        ConstVecIt  cvit;

        explicit BidirectionalIterator(ListT const * data, ConstListIt clit, ConstVecIt  cvit)
            : data(data)
              , clit(clit)
              , cvit(cvit)
        {
        }

        explicit BidirectionalIterator() = default;

        BidirectionalIterator(BidirectionalIterator const & other)
            : data(other.data)
              , clit(other.clit)
              , cvit(other.cvit)
        {
        }

        void swap(BidirectionalIterator & other) noexcept
        {
            using std::swap;
            swap(data, other.data);
        }

        BidirectionalIterator & operator++()
        {
            if(++cvit == clit->cend())
                cvit = (++clit)->cbegin();
            return *this;
        }

        BidirectionalIterator operator++(int)
        {
            BidirectionalIterator tmp(*this);
            ++(*this);
            return tmp;
        }

        BidirectionalIterator & operator--()
        {
            if(cvit == clit->cbegin())
                cvit = (--clit)->cend();
            --cvit;
            return *this;
        }

        BidirectionalIterator operator--(int)
        {
            BidirectionalIterator tmp(*this);
            --(*this);
            return tmp;
        }

        T const * operator->() const
        {
            return &(*cvit);
        }

        T const & operator*() const
        {
            return *cvit;
        }

        bool operator==(BidirectionalIterator const & other) const
        {
            return (cvit == other.cvit);
        }

        bool operator!=(BidirectionalIterator const & other) const
        {
            return !(*this == other);
        }
    };

    using const_iterator = BidirectionalIterator;
    const_iterator begin() const
    {
        if (data_.cbegin() == data_.cend())
            return const_iterator();
        return const_iterator(&data_, data_.cbegin(), data_.cbegin()->cbegin());
    }
    const_iterator end() const
    {
        if (data_.cbegin() == data_.cend())
            return const_iterator();
        return const_iterator(&data_, data_.cend(), data_.cend()->cbegin());
    }

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }
};

void runTest();
