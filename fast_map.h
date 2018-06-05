// Todo: Make this fully drag-n-drop compatible with c++ map/unordered_map.


#include <functional>
#include <vector>
#include <iostream>

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
    m_loadFactor(0.7)
    {

    }

    void reserve(size_t size)
    {
        // Todo: make newSize the next biggest prime?
        size_t newSize = (size_t(size / m_loadFactor));
        m_map.resize(newSize);
        m_free.resize(newSize, true);
    }

    T& operator[] (const Key& k)
    {
        size_t hashValue = hasher(k);

        size_t idx = hashValue % m_map.size();

        while ( (!m_free[idx]) || m_map[idx].key == k)
        {
            // This simply iterates over the rest of the keys to find the next available spots.
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
        }
        m_free[idx] = false;
        return node.value;
    }

private:
    // The underlying array structure which actually contains the values.
    std::vector<fast_map_node<Key, T>> m_map;
    
    // Spots in hash table which are free.
    std::vector<bool> m_free;

    // There is probably something better than std::hash... but use it for now.
    std::hash<Key> hasher;
    double m_loadFactor;
};