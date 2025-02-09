/**
 * @file main.cpp
 * @author TheResplandor
 * @brief Simple manual sanity tests for the key cache.
 */

#include <iostream>
#include <memory>

#include "key_cache.hpp"

using namespace std;

int main()
{
    LURCache<string, int> cache { 3 };
    key_cache_status status = key_cache_status::UNINITIALIZED;
    int val = 0;
    int val7 = 7;

    cache.add("hi", 1);
    cache.add("bi", 2);
    cache.add("kululu", 3);

    status = cache.get("bi", val);
    if (status != key_cache_status::SUCCESS) {
        cout << "failed 1";
        return 1;
    }
    cout << "val = " << val << "\n";

    cache.add("bi", 2233);
    status = cache.get("bi", val);
    if (status != key_cache_status::SUCCESS) {
        cout << "failed 2";
        return 1;
    }
    cout << "val = " << val << "\n";

    cache.add("new1", 5);
    cache.add("new2", 6);
    cache.add("new3", val7);

    status = cache.get("bi", val);
    if (status != key_cache_status::SUCCESS) {
        cout << "failed 3\n";
    } else {
        cout << "val = " << val << "\n";
    }

    val7 = 777;
    status = cache.get("new3", val);
    if (status != key_cache_status::SUCCESS) {
        cout << "failed 4\n";
    } else {
        cout << "val = " << val << "\n";
    }

    return 0;
}