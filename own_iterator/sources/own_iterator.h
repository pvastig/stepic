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

    template<class Type, class ListIt, class VecIt,
             class UnqualifiedType = std::remove_cv_t<Type>>
    struct BidirectionalIterator : std::iterator<std::bidirectional_iterator_tag, UnqualifiedType>
    {
        ListT const * data = nullptr;
        ListIt lit;
        VecIt  vit;

        explicit BidirectionalIterator(ListT const * data, ListIt lit, VecIt vit)
            : data(data)
              , lit(lit)
              , vit(vit)
        {
        }

        explicit BidirectionalIterator() = default;

        BidirectionalIterator(BidirectionalIterator const & other)
            : data(other.data)
              , lit(other.lit)
              , vit(other.vit)
        {
        }

        void swap(BidirectionalIterator & other) noexcept
        {
            using std::swap;
            swap(data, other.data);
        }

        BidirectionalIterator & operator++()
        {
            if(++vit == lit->end())
                vit = (++lit)->begin();
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
            if(vit == lit->begin())
                vit = (--lit)->end();
            --vit;
            return *this;
        }

        BidirectionalIterator operator--(int)
        {
            BidirectionalIterator tmp(*this);
            --(*this);
            return tmp;
        }

        /*T const * operator->() const
        {
            return &(*vit);
        }

        T const & operator*() const
        {
            return *vit;
        }*/

        T & operator*()
        {
            return *vit;
        }

        T * operator->()
        {
            return &(*vit);
        }

        bool operator==(BidirectionalIterator const & other) const
        {
            return vit == other.vit;
        }

        bool operator!=(BidirectionalIterator const & other) const
        {
            return !(*this == other);
        }
    };

    using iterator = BidirectionalIterator<T, typename ListT::iterator, typename VectT::iterator>;
    using const_iterator = BidirectionalIterator<T const, typename ListT::const_iterator, typename VectT::const_iterator>;
    iterator begin()
    {
        if (data_.cbegin() == data_.cend())
            return iterator();
        return iterator(&data_, data_.begin(), data_.begin()->begin());
    }

    iterator end()
    {
        if (data_.cbegin() == data_.cend())
            return iterator();
        return iterator(&data_, data_.end(), data_.end()->begin());
    }

    const_iterator begin() const
    {
        return const_iterator(&data_, data_.cbegin(), data_.cbegin()->cbegin());
    }

    const_iterator end() const
    {
        return const_iterator(&data_, data_.cend(), data_.cend()->cbegin());
    }

    const_iterator cbegin() const
    {
        return begin();
    }

    const_iterator cend() const
    {
        return end();
    }

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend()   { return reverse_iterator(begin()); }

    const_reverse_iterator rbegin() const { return const_reverse_iterator(cend()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(cbegin()); }
};

void runTest();
