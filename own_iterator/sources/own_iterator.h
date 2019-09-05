#include <algorithm>
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


    struct BidirectionalIterator : std::iterator<std::bidirectional_iterator_tag, T const>
    {
        ListT const * data = nullptr;
        using ConstListIt = typename ListT::const_iterator;
        using ConstVecIt = typename VectT::const_iterator;
        ConstListIt clit;
        ConstVecIt  cvit;

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

        void swap(BidirectionalIterator & other) noexcept
        {
            using std::swap;
            swap(data, other.data);
        }

        BidirectionalIterator & operator++()
        {
            auto & vec = *clit;
            ++cvit;
            if (cvit == vec.cend() && vec != data->back())
            {
                ++clit;
                cvit = clit->cbegin();
            }

            if (cvit == vec.cend() && vec == data->front())
            {
                ++clit;
                cvit = vec.cbegin();
            }
            if (cvit == vec.cend() && vec == data->back())
            {
                ++clit;
                auto & vec = *clit;
                cvit = vec.cend();
                return *this;
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
            if (clit == data->cend())
            {
                --clit;
                auto & vec = *clit;
                cvit = vec.cend();
                //--cvit;
                //return *this;
            }
            auto & vec = *clit;
            if (cvit == vec.cbegin() && vec != data->front())
            {
                --clit;
                auto & vec = *clit;
                cvit = vec.cend();
            }

            if (cvit == vec.cbegin() && vec == data->front())
            {
                --cvit;
                return *this;
            }
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
            cout << "==" << boolalpha << (clit == other.clit) << " " << (cvit == other.cvit) << endl;
            return clit == other.clit && cvit == other.cvit;
        }

        bool operator!=(BidirectionalIterator const & other) const
        {
            //cout << &clit << " " << &other.clit << endl;
            cout << "!=" << boolalpha << (clit != other.clit) << " " << (cvit != other.cvit) << endl;
            return clit != other.clit && cvit != other.cvit;
        }

    };

    using const_iterator = BidirectionalIterator;
    const_iterator begin() const {
        if(data_.empty())
            return const_iterator();
        return const_iterator(&data_, data_.cbegin(), data_.cbegin()->cbegin());}
    const_iterator end()   const {
        if(data_.empty())
            return const_iterator();
        return const_iterator(&data_, data_.cend(), --data_.cend()->cend()); }

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }
};

void runTest()
{
    {
       /* VectorList<int> vl;
        std::vector<int> v{};
        vl.append(v.begin(), v.end());
        auto b = vl.begin();
        //cout << *b << endl;
        cout << std::distance(vl.begin(), vl.end()) << endl;*/
    }

    {
        VectorList<int> vl;
        vector<int> v1 {1, 2};
        std::vector<int> v2 {2, 3, 4};
        vector<int> v3 {5, 6};
        cout << "\n----------------\n";
        vl.append(v1.begin(), v1.end());
        vl.append(v2.begin(), v2.end());
        vl.append(v3.begin(), v3.end());
        std::cout << "Test ++i?" << std::endl;
        for (auto i = vl.rend(); i != vl.rbegin(); --i)
        //for (auto i = vl.end(); i != vl.begin(); --i)
        //for (auto i = vl.begin(); i != vl.end(); ++i)
        {
            cout << *i << endl;
        }
        cout << endl;
        cout << std::distance(vl.begin(), vl.end()) << endl;
        cout << *vl.begin() << " end " << *--vl.end() << endl;

        {
            /*VectorList<int> vl1;
            vector<int> v1 {10, 8, 20};
            std::vector<int> v2 {2, 3, 4};
            vector<int> v3 {5, 6};
            cout << "\n----------------\n";
            vl1.append(v1.begin(), v1.end());
            vl1.append(v2.begin(), v2.end());
            vl1.append(v3.begin(), v3.end());
            std::cout << "Test --i" << std::endl;
            for (auto i = vl1.rend(); i != vl1.rbegin(); --i)
            {
                cout << *i << endl;
            }
            cout << std::distance(vl1.begin(), vl1.end()) << endl;*/

        }
        /*cout << endl;
        auto it = vl.begin();
        //auto it = vl.rbegin();
        cout << *it << " ";
        cout << *++it << " ";
        cout << *++it << " ";
        cout << *++it << " ";
        cout << *++it << " ";
        cout << *++it << " ";
        cout << *++it << " ";
        cout << *++it << " ";
        cout << *++it << " ";
        cout << *++it << " ";
        cout << *++it << " ";
        cout << *++it << " ";
        cout << *++it << " ";
        cout << *++it << " ";
        cout << endl;
        cout << *it << " ";
        cout << *--it << " ";
        cout << *--it << " ";
        cout << *--it << " ";
        cout << *--it << " ";
        cout << *--it << " ";
        cout << *--it << " ";
        cout << *--it << " ";
        cout << *--it << " ";
        cout << *--it << " ";
        cout << *--it << " ";
        cout << *--it << " ";
        cout << *--it << " ";
        cout << *--it << " ";
        cout << *--it << " ";*/
    }

    /*VectorList<int> vlist;
    std::vector<int> expectedItems = { 1,2,3,4,5,6,7 };
    std::vector<int> v6;
    v6.push_back(1);
    v6.push_back(2);
    v6.push_back(3);

    std::vector<int> v2;
    v2.push_back(4);
    v2.push_back(5);
    v2.push_back(6);
    v2.push_back(7);
    vlist.append(v6.begin(), v6.end());
    vlist.append(v2.begin(), v2.end());

    std::cout << "reverse distance = " << std::distance(vlist.rbegin(), vlist.rend()) << std::endl;
    std::vector<int> v {1, 2, 3, 4, 5};
    for (auto erit = vlist.rend(); erit != vlist.rbegin();) {
        auto a = *(--erit);
        std::cout << a << endl;
    }

    //Initial test group
    assert( vlist.size() == 7 );
    assert( *vlist.begin() == 1 );
    auto dist = std::distance(vlist.begin(), vlist.end());
    assert( std::distance( vlist.begin(), vlist.end() ) == 7 );

    assert( std::equal(vlist.begin() , vlist.end(), expectedItems.begin() ));

    VectorList<int> vlistEmpty;
    assert(std::distance(vlistEmpty.begin(), vlistEmpty.end()) == 0);

    std::cout << "Test i++" << std::endl;
    for ( auto i = vlist.begin(); i == vlist.end(); i++)
        std::cout << *i << " ";

    std::cout << "Test ++i" << std::endl;
    for (auto i = vlist.begin(); i != vlist.end(); ++i)
        std::cout << *i << " ";

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Test --i" << std::endl;
    for ( auto i = vlist.end(); i != vlist.begin();)
        std::cout << *--i << " ";
    std::cout << std::endl;

    std::cout << "Test i--" << std::endl;
    for (auto i = vlist.end(); i != vlist.begin();) {
        i--;
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    auto j = vlist.rbegin();
    std::cout << "rbegin is " << *j << std::endl;
    j = --vlist.rend();
    std::cout << "--rend is " << *j << std::endl;

    std::cout << "Test reverse_const_iterator ++" << std::endl;
    for (auto j = vlist.rbegin(); j != vlist.rend(); ++j)
        std::cout << *j << " ";
    std::cout << std::endl;

    auto it1 = vlist.begin();
    for ( ; it1 != vlist.end(); ++it1 )
        std::cout << *it1 << ' ';

    std::cout << std::endl;
    std::cout << *--it1 << ' ';
    std::cout << *--it1 << ' ';
    std::cout << *--it1 << ' ';
    std::cout << *--it1 << ' ';

    std::cout << "\nOne element test" << std::endl;
    VectorList<int> vlistOneElement;
    std::vector<int> vOne;
    vOne.push_back( 1 );
    vlistOneElement.append(vOne.begin(), vOne.end() );

    auto it3 = vlistOneElement.begin();
    for (; it3 != vlistOneElement.end(); ++it3)
        std::cout << *it3 << ' ';

    VectorList<int> vListEmpty;
    auto it4 = vListEmpty.begin();
    for (; it4 != vListEmpty.end(); ++it4)
        std::cout << *it4 << ' ';

    cout << endl;
    {
        VectorList<int> vListSingleItem;
        std::vector<int> vOneelemtn;
        vOneelemtn.push_back(1);
        vOneelemtn.push_back(2);
        vListSingleItem.append(vOneelemtn.begin(), vOneelemtn.end());
        vListSingleItem.append(vOneelemtn.begin(), vOneelemtn.end());

        auto itcontaint = vListSingleItem.begin();
        for (; itcontaint != vListSingleItem.end(); ++itcontaint)
            std::cout << *itcontaint << ' ';

        cout << endl;
        --itcontaint;
        std::cout << *itcontaint << ' ';
        --itcontaint;
        std::cout << *itcontaint << ' ';
        --itcontaint;
        std::cout << *itcontaint << ' ';
        --itcontaint;
        std::cout << *itcontaint << ' ';
        --itcontaint;
        //std::cout << *itcontaint << ' ';

    }
    {
        std::vector<std::string> v1 = { "one", "two", "three" };
        std::vector<std::string> v2 = { "four", "five", "six", "seven", "eight" };
        std::vector<std::string> v3 = { "nine", "ten", "eleven", "twelve" };
        std::vector<std::string> v4 = {};
        for (int k = 13; k <= 30; ++k) {
            v4.push_back(std::to_string(k) + "-th");
        }

        VectorList<std::string> vl;
        std::cout << "\nempty distance = " << std::distance(vl.rbegin(), vl.rend()) << std::endl;
        vl.append(v1.begin(), v1.end());
        vl.append(v2.begin(), v2.end());
        vl.append(v3.begin(), v3.end());
        vl.append(v4.begin(), v4.end());
        std::cout << std::endl; std::cout << "distance = " << std::distance(vl.begin(), vl.end()) << std::endl;
        auto it = vl.begin();
        for (; it != vl.end(); ++it) {
            std::string a = *it;   std::cout << a << " ";
        }
        std::cout << std::endl; std::cout << "distance = " << std::distance(vl.begin(), vl.end()) << std::endl;
        VectorList<std::string>::const_iterator eit = vl.end();
        for (; eit != vl.begin();) {
            std::string a = *(--eit);
            std::cout << a << " ";
        }
        std::cout << std::endl;
        auto rit = vl.rend();
        for (; rit != vl.rend(); --rit) {
            std::string a = *rit;   std::cout << a << " ";
        }

        std::cout << std::endl;

        std::cout << "reverse distance = " << std::distance(vl.rbegin(), vl.rend()) << std::endl;
        std::vector<int> v {1, 2, 3, 4, 5};
        for (auto erit = vl.rend(); erit != vl.rbegin();) {
            auto a = *(--erit);
            std::cout << a << endl;
        }

        std::cout << std::endl;
        auto i = vl.rend().base();
        for (; i != vl.end(); ++i) {
            std::string a = *i;
            std::cout << a << " ";
        }
    }*/
}
