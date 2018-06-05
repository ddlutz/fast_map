#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "fast_map.h"

const int c_numStrings{ 10000 };

void unordered_map_test(const std::vector<std::string>& strings);
void map_test(const std::vector<std::string>& strings);
void vector_test(const std::vector<std::string>& strings);
void fasttable_test(const std::vector<std::string>& strings);

int main()
{
    std::vector<std::string> strings;
    strings.reserve(c_numStrings);

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(97, 122);
    auto randChar = std::bind(distribution, generator);

    for(int i = 0; i < c_numStrings; i++)
    {
        const int c_numCharsPerStr{ 4 };

        std::string tmpStr(4, 0);
        for(int j = 0; j < c_numCharsPerStr; j++)
        {
            tmpStr[j] = (char)randChar();
        }
        strings.emplace_back(std::move(tmpStr)); 
    }

    std::cout << "Num of strings in vector: " << strings.size() << std::endl;

    auto begin = std::chrono::system_clock::now();
    unordered_map_test(strings);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = end-begin;
    std::cout << "Unordered map test took: " << duration.count() << " seconds." << std::endl;
    
    begin = std::chrono::system_clock::now();
    map_test(strings);
    end = std::chrono::system_clock::now();
    duration = end-begin;
    std::cout << "Map test took: " << duration.count() << " seconds." << std::endl;

    /*
    begin = std::chrono::system_clock::now();
    vector_test(strings);
    end = std::chrono::system_clock::now();
    duration = end-begin;
    std::cout << "Vector test took: " << duration.count() << " seconds." << std::endl;
    */

    begin = std::chrono::system_clock::now();
    fasttable_test(strings);
    end = std::chrono::system_clock::now();
    duration = end-begin;
    std::cout << "Fast map test took: " << duration.count() << " seconds." << std::endl;
   
}

void unordered_map_test(const std::vector<std::string>& strings)
{
    std::unordered_map<std::string, int> strMap;
    strMap.reserve(c_numStrings);
    for(const auto& str : strings)
    {
        strMap[str]++;
    }
}

void fasttable_test(const std::vector<std::string>& strings)
{
    fast_map<std::string, int> strMap;
    strMap.reserve(c_numStrings);
    for(const auto& str : strings)
    {
        strMap[str]++;
    }
}

void map_test(const std::vector<std::string>& strings)
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