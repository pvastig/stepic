#include "own_iterator.h"

#include <algorithm>
#include <iostream>

void runTest()
{
    {
        //empty case
        VectorList<int> vl;
        std::vector<int> v{};
        vl.append(v.begin(), v.end());
        auto b = vl.begin();
        assert(b == vl.begin());
        assert(std::distance(vl.begin(), vl.end()) == 0);
    }
    {
        /*VectorList<int> vl;
        vector<int> v1 {1};
        vl.append(v1.begin(), v1.end());
        std::cout << "Test one list.vector = 1\n" << std::endl;
        {
            auto it = vl.begin();
            assert(*it == 1);
            ++it;
            assert(it == vl.end());
            --it;
            assert(*it == 1);
            assert(it == vl.begin());
        }
        {
            auto it = vl.end();
            assert(it == vl.end());
            --it;
            assert(*it == 1);
            assert(it == vl.begin());
            ++it;
            assert(it == vl.end());
        }
        {
            auto it = vl.rbegin();
            assert(*it == 1);
            assert(++it == vl.rend());
            --it;
            assert(*it == 1);
            assert(it == vl.rbegin());
        }
        {
            auto it = vl.rend();
            assert(it == vl.rend());
            --it;
            assert(*it == 1);
            assert(it == vl.rbegin());
            ++it;
            assert(it == vl.rend());
        }

        assert(std::distance(vl.begin(), vl.end()) == 1);
        assert(std::distance(vl.rbegin(), vl.rend()) == 1);
        for (auto i = vl.begin(); i != vl.end(); ++i)
            assert(*i == 1);
        for (auto i = vl.rbegin(); i != vl.rend(); ++i)
            assert(*i == 1);
        for (auto i = vl.rend(); i != vl.rbegin(); --i)
            assert(*i == 1);
        for (auto i = vl.end(); i != vl.begin(); --i)
            assert(i == vl.end());

        auto found = std::find(vl.begin(), vl.end(), 1);
        assert(*found == 1);
        found = std::find(vl.begin(), vl.end(), 10);
        assert(found == vl.end());
*/
    }

    {
        VectorList<int> vl;
        vector<int> v1 {1, 2, 3};
        vl.append(v1.begin(), v1.end());
        std::cout << "Test one list.vector = 3\n" << std::endl;
        /*{
            auto it = vl.begin();
            assert(*it == 1);
            ++it;
            ++it;
            assert(it == vl.end());
            --it;
            --it;
            assert(*it == 1);
            assert(it == vl.begin());
        }
        {
            auto it = vl.end();
            assert(it == vl.end());
            --it;
            --it;
            assert(*it == 1);
            assert(it == vl.begin());
            ++it;
            ++it;
            assert(it == vl.end());
        }
        {
            auto it = vl.rbegin();
            assert(*it == 2);
            ++it;
            ++it;
            assert(it == vl.rend());
            --it;
            --it;
            assert(*it == 2);
            assert(it == vl.rbegin());
        }
        {
            auto it = vl.rend();
            assert(it == vl.rend());
            --it;
            --it;
            assert(*it == 2);
            assert(it == vl.rbegin());
            ++it;
            ++it;
            assert(it == vl.rend());
        }*/

        /*assert(std::distance(vl.begin(), vl.end()) == 3);
        assert(std::distance(vl.rbegin(), vl.rend()) == 3);
        std::vector<int> v {1, 2, 3};
        assert(std::equal(vl.begin(), vl.end(), v.begin()));
        assert(std::equal(vl.rbegin(), vl.rend(), v.rbegin()));*/
        size_t count = 0;
        std::vector<int> v {1, 2, 3};
        /*for (auto i = vl.rend(); i != vl.rbegin(); --i)
        {
            cout << *i << endl;
            //assert(*i == v[count++]);
        }*/
        count = 0;
        for (auto i = vl.end(); i != vl.begin(); --i)
        {
            cout << *i << endl;
            //assert(*i == v[++count]);
        }

        /*for (auto i = vl.rbegin(); i != vl.rend(); ++i)
            cout << *i << " ";*/

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
        }*/
    }
