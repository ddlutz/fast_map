#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include <typeinfo>

#include "fast_map.h"

const int c_numStrings{ 1000000 };

void unordered_map_test(const std::vector<std::string>& strings);
void orderedmap_test(const std::vector<std::string>& strings);
void vector_test(const std::vector<std::string>& strings);
void fasttable_test(const std::vector<std::string>& strings);

void test_fasttable();

template <typename Map>
double test_map(Map& map, const std::vector<std::string>& strings, bool reserve)
{
    if (reserve)
    {
        map.reserve(strings.size());
    }
    
    auto begin = std::chrono::system_clock::now();
    for(const auto& str : strings)
    {
        map[str]++;
    }

    int total = 0;
    for(int i = 0; i < 3; i++)
    {
        for(const auto& str : strings)
        {
            total += map[str];
        }
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = end-begin;
    std::cout << typeid(map).name() << " took " << duration.count() << " seconds. Reserved? " << (reserve ? "yes" : "no") << std::endl;
    return duration.count();
}

int main()
{
    std::vector<std::string> strings;
    strings.reserve(c_numStrings);

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(97, 122);
    auto randChar = std::bind(distribution, generator);

    for(int i = 0; i < c_numStrings; i++)
    {
        const int c_numCharsPerStr{ 10 };

        std::string tmpStr(c_numCharsPerStr, 0);
        for(int j = 0; j < c_numCharsPerStr; j++)
        {
            tmpStr[j] = (char)randChar();
        }
        strings.emplace_back(std::move(tmpStr)); 
    }


    fast_map<std::string, int> fastMap, fastMap2;
    std::unordered_map<std::string, int> uMap, uMap2;
    test_map<fast_map<std::string, int>>(fastMap, strings, true);
    test_map<fast_map<std::string, int>>(fastMap2, strings, false);

    test_map<std::unordered_map<std::string, int>>(uMap, strings, true);
    test_map<std::unordered_map<std::string, int>>(uMap2, strings, false);
}

void test_fasttable()
{
    fast_map<std::string, int> map;
    map["hello"] = 1;
    map["hello"]++;
    std::cout << map["hello"] << std::endl;
}

void unordered_map_test(const std::vector<std::string>& strings)
{
    std::unordered_map<std::string, int> strMap;
    //strMap.reserve(c_numStrings);
    for(const auto& str : strings)
    {
        strMap[str]++;
    }

    int total = 0;
    for(int i = 0; i < 10; i++)
    {
        for(const auto& str : strings)
        {
            total += strMap[str];
        }
    }

    //std::cout << "total: " << total << std::endl;
}

void fasttable_test(const std::vector<std::string>& strings)
{
    fast_map<std::string, int> strMap;
    //strMap.reserve(c_numStrings);
    for(const auto& str : strings)
    {
        strMap[str]++;
    }

    
    int total = 0;
    for(int i = 0; i < 10; i++)
    {
        for(const auto& str : strings)
        {
            total += strMap[str];
        }
    }

    //std::cout << "total: " << total << std::endl;
    
}

void orderedmap_test(const std::vector<std::string>& strings)
{
    std::map<std::string, int> strMap;
    for(const auto& str : strings)
    {
        strMap[str]++;
    }
}

void vector_test(const std::vector<std::string>& strings)
{
    int i{ 0 };
    std::vector<std::pair<std::string, int>> vecMap;
    vecMap.reserve(c_numStrings);
    for(const auto& str : strings)
    {
        i++;
        if (i % 5000 == 0) { std::cout << "Currently processing string #" << i << std::endl;
        std::cout << "Size of vector is: " << vecMap.size() << std::endl; }
        bool found{ false };
        // loop through array to find matching string. if one exists, increment. Otherwise, add.
        for(auto& strCountPair : vecMap)
        {
            if (strCountPair.first == str)
            {
                strCountPair.second++;
                found = true; 
                break;
            }
        }
        if (!found)
        {
            vecMap.emplace_back(str, 1);
        }
    }
}