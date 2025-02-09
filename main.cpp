/**
 * @file main.cpp
 * @author TheResplandor
 * @brief Simple manual sanity tests for the key cache.
 */

#include <iostream>
#include <memory>

#include "LRUCache.hpp"

using namespace std;

int main()
{
    LRUCache<string, int> cache { 3 };
    int val7 = 12;

    cache.add("hi", 1);
    cache.add("bi", 2);
    cache.add("kululu", 3);

    auto out1 = cache.get("bi");
    if (!out1) {
        cout << "failed 1";
        return 1;
    }
    auto& val1 = *out1;
    cout << "val1 = " << val1 << "\n";

    cache.add("bi", 2233);
    auto out2 = cache.get("bi");
    if (!out2) {
        cout << "failed 2";
        return 1;
    }
    auto& val2 = *out2;
    cout << "val2 = " << val2 << "\n";

    cache.add("new1", 5);
    cache.add("new2", 6);
    cache.add("new3", val7);

    auto out3 = cache.get("bi");
    if (!out3) {
        cout << "failed 3\n";
    } else {
        auto& val3 = *out3;
        cout << "val3 = " << val3 << "\n";
    }

    val7 = 777;
    auto out4 = cache.get("new3");
    if (!out4) {
        cout << "failed 4\n";
    } else {
        auto& val4 = *out4;
        cout << "val4 = " << val4 << "\n";
    }

    return 0;
}