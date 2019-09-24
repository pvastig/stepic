#include <list>
#include <vector>
#include <cassert>
#include <iostream>

using namespace std;

template<class T>
class VectorList
{
private:
    using VectT  = std::vector<T>;
    using ListT = std::list<VectT>;
    ListT data;

public:
    using value_type = T;

    VectorList() = default;
    VectorList(VectorList const &) = default;
    VectorList(VectorList &&) = default;

    VectorList & operator=(VectorList &&)     = default;
    VectorList & operator=(VectorList const &) = default;

    // метод, который будет использоваться для заполнения VectorList
    // гарантирует, что в списке не будет пустых массивов
    template<class It>
    void append(It p, It q) // определена снаружи
    {
        if (p != q)
            data.push_back(VectT(p,q));
    }

    bool empty() const { return size() == 0; }

    size_t size() const
    {
        size_t s = 0;
        for (auto const & item : data)
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
            if (data->cend() == data->cbegin())
                return *this;

            assert(!data->empty());
            auto & vec = *clit;
            if (cvit != vec.cend())
                ++cvit;
            /*if (cvit == vec.cend() && vec != data->back())
            {
                ++clit;
                cvit = clit->cbegin();
            }*/

            if (cvit == vec.cend() && vec == data->back())
            {
                clit = data->cend();
                cvit = data->cend()->cend();
            }
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
            if (data->cend() == data->cbegin())
                return *this;

            if (clit == data->cend())
            {
                --clit;
                auto & vec = *clit;
                cvit = vec.cend();
                //return *this;
            }
            auto & vec = *clit;
            /*if (cvit == vec.cbegin() && vec != data->front())
            {
                --clit;
                auto & vec = *clit;
                cvit = vec.cend();
                //assert(0);
            }*/
            if (cvit == vec.cbegin() && vec == data->front())
            {
                clit = data->begin();
                cvit = data->begin()->begin();
            }

            //if (cvit != vec.cbegin())
            {
                --cvit;
                //cout << *cvit << " ";
            }
           // --cvit;
            //cout << *cvit << " ";
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
            //cout << "\n==" << boolalpha << (clit == other.clit) << " " << (cvit == other.cvit) << endl;
            return (clit == other.clit) && (cvit == other.cvit);
        }

        bool operator!=(BidirectionalIterator const & other) const
        {
            //cout << "\n!=" << boolalpha << (clit != other.clit) << " " << (cvit != other.cvit) << endl;
            return (clit != other.clit) || (cvit != other.cvit);
        }
    };

    using const_iterator = BidirectionalIterator;
    const_iterator begin() const
    {
        if (data.cbegin() == data.cend())
            return const_iterator();
        return const_iterator(&data, data.cbegin(), data.cbegin()->cbegin());
    }
    const_iterator end() const
    {
        if (data.cbegin() == data.cend())
            return const_iterator();
        return const_iterator(&data, data.cend(), data.cend()->cend());
    }

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }
};

void runTest();
