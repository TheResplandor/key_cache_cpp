#ifndef KEY_CACHE_HPP
#define KEY_CACHE_HPP

#include <concepts>
#include <expected>
#include <list>
#include <mutex>
#include <shared_mutex>
#include <tuple>
#include <unordered_map>
#include <utility>

template <typename T>
concept Hashable = requires(T t) {
    { std::hash<T> {}(t) } -> std::convertible_to<std::size_t>;
};

enum class key_cache_status {
    UNINITIALIZED = -1,
    SUCCESS = 0,
    EXISTING_KEY_NOT_FOUND,
    KEY_NOT_FOUND,
};

/**
 * @brief Key based size limited thread safe least-recently-used cache.
 *
 * @tparam KT Type of keys to identify values in the cache. Must be hashable.
 * @tparam VT Type of values to be stored in the cache.
 */
template <Hashable KT, typename VT>
class LURCache {
public:
    LURCache(size_t capacity):
        m_capacity { capacity },
        m_size { 0 },
        m_order {},
        m_vals {},
        m_mtx {}
    {
    }

    /**
     * @brief Add a value to the cache.
     *
     * @param[in] key The unique key of the new value.
     * @param[in] val The value to be store with the key.
     * @return key_cache_statuses
     */
    key_cache_status add(KT const& key, VT const& val)
    {
        std::unique_lock<std::shared_mutex> wlock(m_mtx);

        auto found_pair = m_vals.find(key);

        if (found_pair != m_vals.end()) {
            // Avoid re-adding existing key-value pairs.
            if (val == found_pair->second.first) {
                return key_cache_status::SUCCESS;
            }
            // Else remove the already existing key and add it as a new one.
            m_order.erase(found_pair->second.second);
        } else if (m_capacity == m_size) {
            // If capacity is full, remove the oldest key and add a new one.
            auto oldest_key = std::prev(m_order.cend());

            size_t elems_removed = m_vals.erase(*oldest_key);
            if (elems_removed != 1) {
                return key_cache_status::EXISTING_KEY_NOT_FOUND;
            }

            m_order.erase(oldest_key);
        } else {
            // We add a new key without removing an old one, so size grows.
            m_size++;
        }

        m_order.push_front(key);
        m_vals[key] = std::pair { val, m_order.begin() };

        return key_cache_status::SUCCESS;
    }

    /**
     * @brief Get a value from the cache.
     *
     * @param[in] key The key of the value to get.
     * @return the value from the cache, or status on error.
     */
    std::expected<VT, key_cache_status> get(KT const& key)
    {
        std::shared_lock<std::shared_mutex> rlock(m_mtx);

        auto found_pair = m_vals.find(key);

        if (found_pair == m_vals.end()) {
            return std::unexpected(key_cache_status::KEY_NOT_FOUND);
        }

        std::tie(val, std::ignore) = found_pair->second;

        return key_cache_status::SUCCESS;
    }

private:
    size_t m_capacity;
    size_t m_size;
    std::list<KT> m_order;
    std::unordered_map<KT,
        typename std::pair<VT, typename std::list<KT>::const_iterator>>
        m_vals;
    std::shared_mutex m_mtx;
};

#endif // KEY_CACHE_HPP