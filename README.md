# Fast_Map
This repo is to explore alternative hash tables to the std::unorderd_map in c++.

For now, a simple fast_map exists which can only do insertions / updates / lookups. There is no way to delete items. Additionally, the table size must be specified ahead of time and cannot be resized. However, it is much faster than the built-in unordered_map. The test program it runs generates 10,000 4 character long strings, and performs a count of each string. Some results, running on a 2017 MBP:

Douglass-MacBook-Pro:HashSpeed ddlutz$ g++ main.cpp -std=c++11 -o test
Douglass-MacBook-Pro:HashSpeed ddlutz$ ./test
Num of strings in vector: 10000
Unordered map test took: 0.005148 seconds.
Map test took: 0.020605 seconds.

Fast map test took: 0.003308 seconds.
Douglass-MacBook-Pro:HashSpeed ddlutz$ g++ main.cpp -std=c++11 -O3  -o test
Douglass-MacBook-Pro:HashSpeed ddlutz$ ./test
Num of strings in vector: 10000
Unordered map test took: 0.00354 seconds.
Map test took: 0.004663 seconds.
Fast map test took: 0.001176 seconds.
