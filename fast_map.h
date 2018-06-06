// Todo: Make this fully drag-n-drop compatible with c++ map/unordered_map.


#include <functional>
#include <vector>
#include <iostream>

namespace
{
    const double c_loadFactor{ 0.89 };

    bool isPrime(size_t n)
    {
        for(int i = 0; i < n / 2; i++)
        {
            return false;
        }
        return true;
    }
}
template <typename Key, typename T>
class fast_map_node
{
public:
    Key key;
    T value;
};

template <typename Key, typename T>
class fast_map
{
public:

    fast_map() :
    m_loadFactor(c_loadFactor),
    m_count(0)
    {
        reserve(m_primes[0]);
    }

    void reserve(size_t size)
    {
        // Todo: make newSize the next biggest prime?
        size_t newSize = (size_t(size / m_loadFactor));
        newSize = find_next_prime(newSize);
        m_map.resize(newSize);
        m_free.resize(newSize, true);
    }

    T& operator[] (const Key& k)
    {
        double loadFactor = static_cast<double>(m_count) / (double)m_map.size();

        if (  loadFactor > m_loadFactor)
        {
            //std::cout << "load factor: " << loadFactor << std::endl;
            resize();
        }

        size_t hashValue = hasher(k);

        size_t idx = hashValue % m_map.size();

        while ( (!m_free[idx]))
        {
            if (m_map[idx].key == k)
            {
                //std::cout << "found a match!" << std::endl;
                break;
            }
            // This simply iterates over the rest of the keys to find the next available spots.
            // Maybe try double hashing, cuckoo hashing, quadtratic probing, etc.
            idx++;
            if (idx >= m_map.size())
            {
                idx = 0;
            }
        }

        fast_map_node<Key, T>& node = m_map[idx];
        if (m_free[idx])
        {
            node.key = k;
            node.value = T();
            m_count++;
            //std::cout << "count: " << m_count << std::endl;
        }
        m_free[idx] = false;
        return node.value;
    }

private:
    size_t find_next_prime(size_t num)
    {
        //std::cout << "previus num: " << num << std::endl;
        for(const auto& prime : m_primes)
        {
            if (prime >= num)
            {
                return prime;
            }
        }

        return 2 * num;
    }

    void resize()
    {
        //std::cout << "resize called" << std::endl;
        size_t nextSize = find_next_prime(m_map.size() + 1);
        //std:: cout << "next size: " << nextSize << std::endl;;
        std::vector<fast_map_node<Key, T>> newMap;
        std::vector<bool> newFrees;
        newFrees.resize(nextSize, true);
        newMap.resize(nextSize);

        for(int i = 0; i < m_map.size(); i++)
        {
            if(!m_free[i])
            {
                size_t hashValue = hasher(m_map[i].key);

                size_t idx = hashValue % nextSize;

                while (!newFrees[idx])
                {
                    // This simply iterates over the rest of the keys to find the next available spots.
                    // Maybe try double hashing, cuckoo hashing, quadtratic probing, etc.
                    idx++;
                    if (idx >= nextSize)
                    {
                        idx = 0;
                    }
                }

                fast_map_node<Key, T>& node = newMap[idx];
                node.key = m_map[i].key;
                node.value = m_map[i].value;
                newFrees[idx] = false;
            }
        }

        m_map = std::move(newMap);
        m_free = std::move(newFrees);
    }

private:
    // The underlying array structure which actually contains the values.
    std::vector<fast_map_node<Key, T>> m_map;
    
    // Spots in hash table which are free.
    std::vector<bool> m_free;

    // There is probably something better than std::hash... but use it for now.
    std::hash<Key> hasher;
    double m_loadFactor;
    size_t m_count;

    const std::vector<size_t> m_primes{ 101, 433, 1877, 7793, 15733 };
};