/**
 * @file main.cpp
 * @author TheResplandor
 * @brief Simple manual sanity tests for the key cache.
 */

#include <iostream>
#include <memory>

#include "key_cache.hpp"

using namespace std;

#define UNUSED(var) (void)(var)

int main(int argc, char* argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    key_cache<char const*, int> cache { 3 };
    key_cache_statuses status = key_cache_statuses::UNINITIALIZED;
    int val = 0;
    int val3 = 7;

    cache.add("hi", 1);
    cache.add("bi", 2);
    cache.add("kululu", 3);

    status = cache.get("bi", val);
    if (status != key_cache_statuses::SUCCESS) {
        cout << "failed 1";
        return 1;
    }
    cout << "val = " << val << "\n";

    cache.add("bi", 2233);
    status = cache.get("bi", val);
    if (status != key_cache_statuses::SUCCESS) {
        cout << "failed 2";
        return 1;
    }
    cout << "val = " << val << "\n";

    cache.add("new1", 5);
    cache.add("new2", 6);
    cache.add("new3", val3);

    status = cache.get("bi", val);
    if (status != key_cache_statuses::SUCCESS) {
        cout << "failed 3\n";
    } else {
        cout << "val = " << val << "\n";
    }

    val3 = 777;
    status = cache.get("new3", val);
    if (status != key_cache_statuses::SUCCESS) {
        cout << "failed 4\n";
    } else {
        cout << "val = " << val << "\n";
    }

    return 0;
}