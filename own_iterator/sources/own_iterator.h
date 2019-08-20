#include <list>
#include <vector>
#include <iostream>
#include <iterator>
#include <utility>
#include <cassert>

using namespace std;

template<class T>
class VectorList
{
private:
    using VectT  = std::vector<T>;
    using ListT = std::list<VectT>;
    ListT data_;

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
        bool isFinished = false;

        explicit BidirectionalIterator(ListT const * data, ConstListIt clit, ConstVecIt cvit)
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

        BidirectionalIterator & operator=(BidirectionalIterator other)
        {
            swap(other);
            return *this;
        }

        void swap(BidirectionalIterator & other) noexcept
        {
            using std::swap;
            swap(data, other.data);
        }

        BidirectionalIterator & operator++()
        {
            auto & vec = *clit;
            //cout << "operator++" << std::boolalpha <<(cvit == vec.cend()) << " " << (clit != data->cend()) << endl;
            ++cvit;
            if (cvit == vec.cend() && clit != data->cend())
            {
                ++clit;
                auto & vec = *clit;
                cvit = vec.begin();
            }
            isFinished = (cvit == vec.cend()) || (clit == data->cend());
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
            if (clit == data->cend())
            {
                --clit;
                auto & vec = *clit;
                cvit = --vec.cend();
                return *this;
            }
            auto & vec = *clit;
            if (cvit == vec.cbegin())
            {

                --clit;
                auto & vec = *clit;
                cvit = vec.cend();
            }
            //cout << "operator--" << std::boolalpha <<(cvit == vec.cbegin()) << " " << (clit == data->cbegin()) << endl;
            //isFinished = (cvit != vec.cbegin()) || (clit != data->cbegin());
             /*if (clit == data->cbegin())
                 return *this;*/
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
            return (clit != other.clit) && (cvit == other.cvit);
        }

        bool operator!=(BidirectionalIterator const & other) const
        {
            //cout << "operator!=" << boolalpha << (cvit != other.cvit) << " " << (clit != other.clit) << endl;
            //return (clit != other.clit) && (cvit != other.cvit);
            //cout << boolalpha << isFinished << endl;
            return (clit != other.clit) && (cvit != other.cvit);
        }

    };

    using const_iterator = BidirectionalIterator;
    const_iterator begin() const { return const_iterator(&data_, data_.cbegin(), data_.cbegin()->cbegin());}
    const_iterator end()   const { return const_iterator(&data_, data_.cend(), data_.cend()->cend()); }

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end());   }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }

    /*template<class Type,
             class UnqualifiedType = std::remove_cv_t<Type>>
    class BiderectionIterator : public std::iterator<std::bidirectional_iterator_tag,
                                                 UnqualifiedType,
                                                 std::ptrdiff_t,
                                                 Type*,
                                                 Type&>
    {
        VectorList<UnqualifiedType> * m_it;
        std::vector<decltype(m_it->data_)> * m_data;

        explicit BiderectionIterator(VectorList<UnqualifiedType> * it)
            : m_it(it), m_data(it->data_)
        {
        }

    public:
        BiderectionIterator() : m_it(nullptr)
        {
        }

        void swap(BiderectionIterator & other) noexcept
        {
            using std::swap;
            swap(m_it, other.iter);
        }

        BiderectionIterator& operator++()
        {
            assert(m_it != nullptr && "Out-of-bounds iterator increment!");
            ++m_it;
            ++m_data;
            return *this;
        }

        BiderectionIterator operator++(int)
        {
            assert(m_it != nullptr && "Out-of-bounds iterator increment!");
            BiderectionIterator tmp(*this);
            m_it++;
            m_data++;
            return tmp;
        }

        // two-way comparison: v.begin() == v.cbegin() and vice versa
        template<class OtherType>
        bool operator==(BiderectionIterator<OtherType> const & rhs) const
        {
            return m_it == rhs.m_it;
        }

        template<class OtherType>
        bool operator != (BiderectionIterator<OtherType> const & rhs) const
        {
            return m_it != rhs.m_it;
        }

        Type & operator*() const
        {
            assert(m_it != nullptr && "Invalid iterator dereference!");
            return (Type&)*m_data;
        }

        Type & operator->() const
        {
            assert(m_it != nullptr && "Invalid iterator dereference!");
            return m_data;
        }

        // One way conversion: iterator -> const_iterator
        operator BiderectionIterator<const Type>() const
        {
            return BiderectionIterator<const Type>(m_it);
        }
    };

    using iterator = BiderectionIterator<T>;
    using const_iterator = BiderectionIterator<const T>;

    iterator begin() { return iterator(this);}
    iterator end() { return iterator();}

    const_iterator begin() const { return const_iterator(this);}
    const_iterator end() const   { return const_iterator(); }*/

/*    // определите const_reverse_iterator
    ... const_reverse_iterator ...

        // определите методы rbegin / rend
        const_reverse_iterator rbegin() const { return ... ;   }
    const_reverse_iterator rend()   const { return ... ; }*/
};

void runTest()
{
    {
        VectorList<int> vl;
        std::vector<int> v{1, 2};
        vl.append(v.begin(), v.end());
        std::vector<int> v2{3, 4};
        vl.append(v2.begin(), v2.end());
        //std::vector<int> v3{5, 6};
        //vl.append(v3.begin(), v3.end());
        std::cout << vl.size() << std::endl;

        auto b =vl.begin();
        std::cout << *b << std::endl;
        b++;
        std::cout << *b << std::endl;
        b++;
        std::cout << *b << std::endl;
        b++;
        std::cout << *b << std::endl;
        b++;
        std::cout << std::boolalpha << (b == vl.end()) << std::endl;

        for (auto item : vl) {
            std::cout << "item: " << item << ' ';
        }

        std::cout << "Test --i" << std::endl;
        //auto it = vl.end();
        //for (auto i = vl.rbegin(); i != vl.rend(); ++i)
        for (auto i = vl.end(); i != vl.begin(); --i)
        //for (auto i = vl.begin(); i != vl.end(); ++i) //{}
            std::cout << *i << " ";
        std::cout << std::endl;
    }

    /*{
        VectorList<int> vl;
        std::vector<int> v{1, 2};
        vl.append(v.begin(), v.end());
        std::vector<int> v2{ 5 };
        vl.append(v2.begin(), v2.end());
        //std::cout << vl.size() << std::endl;

        auto b = vl.end();
        --b;
        std::cout << *b << std::endl;
        --b;
        std::cout << *b << std::endl;
        --b;
        std::cout << *b << std::endl;

        for (auto const & item : vl) {
            std::cout << "item: " << item << ' ';
        }
    }*/

   /* VectorList<char> vlist;

    std::vector<char> v1;
    v1.push_back('A');
    v1.push_back('B');
    v1.push_back('C');
    v1.push_back('K');

    std::vector<char> v2;
    v2.push_back('D');
    v2.push_back('E');
    v2.push_back('F');
     std::vector<char> v3;
    v2.push_back('G');
    v2.push_back('H');
    std::vector<char> v4;
    v2.push_back('L');
    v2.push_back('M');
    vlist.append(v1.begin(), v1.end());
    vlist.append(v2.begin(), v2.end());
    vlist.append(v3.begin(), v3.end());
    vlist.append(v4.begin(), v4.end());

    auto i = vlist.begin();
    /*std::cout << "Size is " << vlist.size() << std::endl;
    std::cout << "begin is " << *i << std::endl;
    std::cout << "std::distance(begin,end)﻿ " << (std::distance(vlist.begin(), vlist.end())) << std::endl;
    std::cout << "*(++begin) == 'B'? " << (*++vlist.begin() == 'B') << std::endl;
    std::cout << "*(++begin) == 'A'? " << (*++vlist.begin() == 'A') << std::endl;
    std::cout << std::endl;

    std::cout << "Test ++i" << std::endl;
    for (i = vlist.begin(); i != vlist.end(); ++i)
        std::cout << *i << " ";
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Test i++" << std::endl;
    for (auto i = vlist.begin(); i != vlist.end(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Test --i" << std::endl;
    for (auto i = vlist.end(); i != vlist.begin();)
        std::cout << *--i << " ";
    std::cout << std::endl;
    /*auto b = vlist.end();
    std::cout << *--b << endl;
    std::cout << *--b << endl;
    std::cout << *--b << endl;
    std::cout << *--b << endl;
    std::cout << *--b << endl;
    std::cout << *--b << endl;
    std::cout << *--b << endl;

    std::cout << "Test i--" << std::endl;
    for (auto i = vlist.end(); i != vlist.begin();) {
        i--;
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << std::endl;
    auto j = vlist.rbegin();
    std::cout << "rbegin is " << *j << std::endl;
    auto k = --vlist.rend();
    std::cout << "++rend is " << *k << std::endl;*/

    /*std::cout << "Test reverse_const_iterator ++" << std::endl;
    for (auto j = vlist.rbegin(); j != vlist.rend(); ++j)
        std::cout << *j << " ";
    std::cout << std::endl;*/

    /*{
        string s("Test");
        std::vector<string> v1;
        v1.push_back("Hello");
        v1.push_back("One");
        v1.push_back("Two");

        std::vector<string> v2;
        v2.push_back("D");
        v2.push_back("E");
        v2.push_back("F");
        VectorList<string> v;
        v.append(v1.begin(), v1.end());
        v.append(v2.begin(), v2.end());
        auto b = v.begin();
        if ( &s  != &*b  ||  s != *b  || s  != b ->c_str())
            cout << "test!" << endl;

        std::string const & s2 = *b;
    }*/
}
