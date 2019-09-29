#include "own_iterator.h"

#include <algorithm>
#include <iostream>

void runTest()
{
    using namespace std;
    {
        //empty case
        VectorList<int> vl;
        cout << "distance:" << distance(vl.begin(), vl.end()) << endl;
        assert(distance(vl.begin(), vl.end()) == 0);
    }
    {
        VectorList<int> vl;
        vector<int> v{};
        vl.append(v.begin(), v.end());
        auto b = vl.begin();
        assert(b == vl.begin());
        cout << "distance:" << distance(vl.begin(), vl.end()) << endl;
        assert(distance(vl.begin(), vl.end()) == 0);
    }
    {
        VectorList<int> vl;
        vector<int> v1 {1};
        vl.append(v1.begin(), v1.end());
        cout << "Test list.vector.size = 1" << endl;
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

        assert(distance(vl.begin(), vl.end()) == 1);
        assert(distance(vl.rbegin(), vl.rend()) == 1);
        for (auto i = vl.begin(); i != vl.end(); ++i)
            assert(*i == 1);
        for (auto i = vl.rbegin(); i != vl.rend(); ++i)
            assert(*i == 1);
        for (auto i = vl.rend(); i != vl.rbegin(); --i)
        {}
         cout << endl;
         for (auto i = vl.end(); i != vl.begin(); --i)
             assert(i == vl.end());

        auto found = find(vl.begin(), vl.end(), 1);
        assert(*found == 1);
        found = find(vl.begin(), vl.end(), 10);
        assert(found == vl.end());
    }

    {
        VectorList<int> vl;
        vector<int> v1 {1, 2, 3};
        vl.append(v1.begin(), v1.end());
        cout << "Test list.vector.size = 3" << endl;
        {
            auto it = vl.begin();
            assert(*it == 1);
            ++it;
            assert(*it == 2);
            ++it;
            assert(*it == 3);
            ++it;
            assert(it == vl.end());
            --it;
            assert(*it == 3);
            --it;
            cout << *it << endl;
            assert(*it == 2);
            --it;
            assert(*it == 1);
            assert(it == vl.begin());
        }
        {
            auto it = vl.end();
            assert(it == vl.end());
            --it;
            assert(*it == 3);
            --it;
            assert(*it == 2);
            --it;
            assert(*it == 1);
            assert(it == vl.begin());
            ++it;
            assert(*it == 2);
            ++it;
            assert(*it == 3);
            ++it;
            assert(it == vl.end());
        }
        {
            auto it = vl.rbegin();
            assert(*it == 3);
            ++it;
            assert(*it == 2);
            ++it;
            assert(*it == 1);
            ++it;
            assert(it == vl.rend());
            --it;
            assert(*it == 1);
            --it;
            assert(*it == 2);
            --it;
            assert(*it == 3);
            --it;
            assert(it == vl.rbegin());
        }
        {
            auto it = vl.rend();
            assert(it == vl.rend());
            --it;
            assert(*it == 1);
            --it;
            assert(*it == 2);
            --it;
            assert(*it == 3);
            assert(it == vl.rbegin());
            ++it;
            assert(*it == 2);
            ++it;
            assert(*it == 1);
            ++it;
            assert(it == vl.rend());
        }

        assert(distance(vl.begin(), vl.end()) == 3);
        assert(distance(vl.rbegin(), vl.rend()) == 3);
        assert(equal(vl.begin(), vl.end(), v1.begin()));
        assert(equal(vl.rbegin(), vl.rend(), v1.rbegin()));

        auto found = find(vl.begin(), vl.end(), 1);
        assert(*found == 1);
        found = find(vl.begin(), vl.end(), 10);
        assert(found == vl.end());
        cout << endl;
    }
    {
        VectorList<int> vl;
        vector<int> v1 {1, 2, 3};
        vector<int> v2 {4, 5, 6};
        vector<int> v3 {7, 8, 9};
        vector<int> comparedValues{1, 2, 3, 4, 5, 6, 7, 8, 9};
        vl.append(v1.begin(), v1.end());
        vl.append(v2.begin(), v2.end());
        vl.append(v3.begin(), v3.end());
        cout << "Test list.size = 3, vector.size = 3" << endl;
        {
            auto it = vl.begin();
            assert(*it == 1);
            ++it;
            assert(*it == 2);
            ++it;
            assert(*it == 3);
            ++it;
            assert(*it == 4);
            ++it;
            assert(*it == 5);
            ++it;
            assert(*it == 6);
            ++it;
            assert(*it == 7);
            ++it;
            assert(*it == 8);
            ++it;
            assert(*it == 9);
            ++it;
            assert(it == vl.end());
            --it;
            assert(*it == 9);
            --it;
            assert(*it == 8);
            --it;
            assert(*it == 7);
            --it;
            assert(*it == 6);
            --it;
            assert(*it == 5);
            --it;
            assert(*it == 4);
            --it;
            assert(*it == 3);
            --it;
            assert(*it == 2);
            --it;
            assert(*it == 1);
            assert(it == vl.begin());
        }
        {
            auto it = vl.end();
            assert(it == vl.end());
            --it;
            assert(*it == 9);
            --it;
            assert(*it == 8);
            --it;
            assert(*it == 7);
            --it;
            assert(*it == 6);
            --it;
            assert(*it == 5);
            --it;
            assert(*it == 4);
            --it;
            assert(*it == 3);
            --it;
            assert(*it == 2);
            --it;
            assert(*it == 1);
            assert(it == vl.begin());
            --it;
            ++it;
            assert(*it == 1);
            ++it;
            assert(*it == 2);
            ++it;
            assert(*it == 3);
            ++it;
            assert(*it == 4);
            ++it;
            assert(*it == 5);
            ++it;
            assert(*it == 6);
            ++it;
            assert(*it == 7);
            ++it;
            assert(*it == 8);
            ++it;
            assert(*it == 9);
            ++it;
            assert(it == vl.end());
        }
        {
            auto it = vl.rbegin();
            assert(*it == 9);
            ++it;
            assert(*it == 8);
            ++it;
            assert(*it == 7);
            ++it;
            assert(*it == 6);
            ++it;
            assert(*it == 5);
            ++it;
            assert(*it == 4);
            ++it;
            assert(*it == 3);
            ++it;
            assert(*it == 2);
            ++it;
            assert(*it == 1);
            ++it;
            assert(it == vl.rend());
            --it;
            assert(*it == 1);
            --it;
            assert(*it == 2);
            --it;
            assert(*it == 3);
            --it;
            assert(*it == 4);
            --it;
            assert(*it == 5);
            --it;
            assert(*it == 6);
            --it;
            assert(*it == 7);
            --it;
            assert(*it == 8);
            --it;
            assert(*it == 9);
            --it;
            assert(it == vl.rbegin());
        }
        {
            auto it = vl.rend();
            assert(it == vl.rend());
            --it;
            assert(*it == 1);
            --it;
            assert(*it == 2);
            --it;
            assert(*it == 3);
            --it;
            assert(*it == 4);
            --it;
            assert(*it == 5);
            --it;
            assert(*it == 6);
            --it;
            assert(*it == 7);
            --it;
            assert(*it == 8);
            --it;
            assert(*it == 9);
            assert(it == vl.rbegin());
            ++it;
            assert(*it == 8);
            ++it;
            assert(*it == 7);
            ++it;
            assert(*it == 6);
            ++it;
            assert(*it == 5);
            ++it;
            assert(*it == 4);
            ++it;
            assert(*it == 3);
            ++it;
            assert(*it == 2);
            ++it;
            assert(*it == 1);
            ++it;
            assert(it == vl.rend());
        }

        assert(distance(vl.begin(), vl.end()) == 9);
        assert(distance(vl.rbegin(), vl.rend()) == 9);
        assert(equal(vl.begin(), vl.end(), comparedValues.begin()));
        assert(equal(vl.rbegin(), vl.rend(), comparedValues.rbegin()));

        auto found = find(vl.begin(), vl.end(), 1);
        assert(*found == 1);
        assert(found == vl.begin());
        found = find(vl.begin(), vl.end(), 9);
        assert(*found == 9);
        assert(++found == vl.end());
        found = find(vl.begin(), vl.end(), 5);
        assert(*found == 5);
        found = find(vl.begin(), vl.end(), 10);
        assert(found == vl.end());
    }


    VectorList<int> vlist;
    vector<int> expectedItems = { 1,2,3,4,5,6,7 };
    vector<int> v6;
    v6.push_back(1);
    v6.push_back(2);
    v6.push_back(3);

    vector<int> v2;
    v2.push_back(4);
    v2.push_back(5);
    v2.push_back(6);
    v2.push_back(7);
    vlist.append(v6.begin(), v6.end());
    vlist.append(v2.begin(), v2.end());

    cout << "reverse distance = " << distance(vlist.rbegin(), vlist.rend()) << endl;
    for (auto erit = vlist.rend(); erit != vlist.rbegin();) {
        auto a = *(--erit);
        cout << a << endl;
    }

    //Initial test group
    assert(vlist.size() == 7);
    assert(*vlist.begin() == 1 );
    assert(distance(vlist.begin(), vlist.end() ) == 7 );

    assert( equal(vlist.begin() , vlist.end(), expectedItems.begin() ));

    VectorList<int> vlistEmpty;
    assert(distance(vlistEmpty.begin(), vlistEmpty.end()) == 0);

    cout << "Test i++" << endl;
    for ( auto i = vlist.begin(); i == vlist.end(); i++)
        cout << *i << " ";

    cout << "Test ++i" << endl;
    for (auto i = vlist.begin(); i != vlist.end(); ++i)
        cout << *i << " ";

    cout << endl;
    cout << endl;

    cout << "Test --i" << endl;
    for ( auto i = vlist.end(); i != vlist.begin();)
        cout << *--i << " ";
    cout << endl;

    cout << "Test i--" << endl;
    for (auto i = vlist.end(); i != vlist.begin();) {
        i--;
        cout << *i << " ";
    }
    cout << endl;
    cout << endl;
    auto j = vlist.rbegin();
    cout << "rbegin is " << *j << endl;
    j = --vlist.rend();
    cout << "--rend is " << *j << endl;

    cout << "Test reverse_const_iterator ++" << endl;
    for (auto j = vlist.rbegin(); j != vlist.rend(); ++j)
        cout << *j << " ";
    cout << endl;

    auto it1 = vlist.begin();
    for ( ; it1 != vlist.end(); ++it1 )
        cout << *it1 << ' ';

    cout << endl;
    cout << *--it1 << ' ';
    cout << *--it1 << ' ';
    cout << *--it1 << ' ';
    cout << *--it1 << ' ';

    cout << "\nOne element test" << endl;
    VectorList<int> vlistOneElement;
    vector<int> vOne;
    vOne.push_back( 1 );
    vlistOneElement.append(vOne.begin(), vOne.end() );

    auto it3 = vlistOneElement.begin();
    for (; it3 != vlistOneElement.end(); ++it3)
        cout << *it3 << ' ';

    VectorList<int> vListEmpty;
    auto it4 = vListEmpty.begin();
    for (; it4 != vListEmpty.end(); ++it4)
        cout << *it4 << ' ';

    cout << endl;
    {
        VectorList<int> vListSingleItem;
        vector<int> vOneelemtn;
        vOneelemtn.push_back(1);
        vOneelemtn.push_back(2);
        vListSingleItem.append(vOneelemtn.begin(), vOneelemtn.end());
        vListSingleItem.append(vOneelemtn.begin(), vOneelemtn.end());

        auto itcontaint = vListSingleItem.begin();
        for (; itcontaint != vListSingleItem.end(); ++itcontaint)
            cout << *itcontaint << ' ';

        cout << endl;
        --itcontaint;
        cout << *itcontaint << ' ';
        --itcontaint;
        cout << *itcontaint << ' ';
        --itcontaint;
        cout << *itcontaint << ' ';
        --itcontaint;
        cout << *itcontaint << ' ';
        --itcontaint;
        //cout << *itcontaint << ' ';

    }
    {
        vector<string> v1 = { "one", "two", "three" };
        vector<string> v2 = { "four", "five", "six", "seven", "eight" };
        vector<string> v3 = { "nine", "ten", "eleven", "twelve" };
        vector<string> v4 = {};
        for (int k = 13; k <= 30; ++k) {
            v4.push_back(to_string(k) + "-th");
        }

        VectorList<string> vl;
        cout << "\nempty distance = " << distance(vl.rbegin(), vl.rend()) << endl;
        vl.append(v1.begin(), v1.end());
        vl.append(v2.begin(), v2.end());
        vl.append(v3.begin(), v3.end());
        vl.append(v4.begin(), v4.end());
        cout << endl; cout << "distance = " << distance(vl.begin(), vl.end()) << endl;
        auto it = vl.begin();
        for (; it != vl.end(); ++it) {
            string a = *it;   cout << a << " ";
        }
        cout << endl; cout << "distance = " << distance(vl.begin(), vl.end()) << endl;
        VectorList<string>::const_iterator eit = vl.end();
        for (; eit != vl.begin();) {
            string a = *(--eit);
            cout << a << " ";
        }
        cout << endl;
        auto rit = vl.rend();
        for (; rit != vl.rend(); --rit) {
            string a = *rit;   cout << a << " ";
        }

        cout << endl;

        cout << "reverse distance = " << distance(vl.rbegin(), vl.rend()) << endl;
        for (auto erit = vl.rend(); erit != vl.rbegin();) {
            auto a = *(--erit);
            cout << a << endl;
        }

        cout << endl;
        auto i = vl.rend().base();
        for (; i != vl.end(); ++i) {
            string a = *i;
            cout << a << " ";
        }
    }
}
