#pragma once

#include <vector>
#if !defined(NDEBUG)
# include <cassert>
#endif

template<typename T, typename Index = size_t>
class unordered_vector {
public:
  using index_t = Index;
  using iterator = typename std::vector<T>::iterator;
  using const_iterator = typename std::vector<T>::const_iterator;

  template<typename... Args>
  index_t emplace(Args&&... args) {
    m_values.emplace_back(std::forward<Args>(args)...);
    const auto index = allocate_index_to_offset();
    m_index_to_offset[index] = m_values.size() - 1;
    m_offset_to_index.emplace_back(index);
    return index;
  }

  void erase(index_t index) {
    const auto offset = get_offset_by_index(index);
    free_index_to_offset(index);
    const auto moved_index = m_offset_to_index[m_values.size() - 1];
    m_index_to_offset[moved_index] = offset;
    m_offset_to_index[offset] = moved_index;
    m_values[offset] = std::move(m_values.back());
    m_values.pop_back();
    m_offset_to_index.pop_back();

  }
  iterator erase(iterator it) {
    const auto offset = std::distance(m_values.begin(), it);
    erase(m_offset_to_index[offset]);
    return it;
  }

  void reserve(size_t count) {
    m_values.reserve(count);
    m_offset_to_index.reserve(count);
    m_index_to_offset.reserve(count);
    m_unused_index_to_offset.reserve(count);
  }

  size_t size() const { return m_values.size(); }

  iterator begin() { return m_values.begin(); }
  const_iterator begin() const { return m_values.begin(); }
  const_iterator cbegin() const { return m_values.cbegin(); }
  iterator end() { return m_values.end(); }
  const_iterator end() const { return m_values.end(); }
  const_iterator cend() const { return m_values.cend(); }

  const T& operator[](index_t index) const { return m_values[get_offset_by_index(index)]; }
  T& operator[](index_t index) { return m_values[get_offset_by_index(index)]; }

private:
  index_t get_offset_by_index(index_t index) const {
#if !defined(NDEBUG)
    assert(index < m_index_to_offset.size());
    assert(m_index_to_offset[index] < m_values.size());
#endif
    return m_index_to_offset[index];
  }

  index_t allocate_index_to_offset() {
    if (!m_unused_index_to_offset.empty()) {
      const auto index = m_unused_index_to_offset.back();
      m_unused_index_to_offset.pop_back();
      return index;
    }
    m_index_to_offset.emplace_back();
    return m_index_to_offset.size() - 1;
  }

  void free_index_to_offset(index_t index) {
#if !defined(NDEBUG)
    m_index_to_offset[index] = ~index_t{ };
#endif
    m_unused_index_to_offset.push_back(index);
  }

  std::vector<T> m_values;
  // TODO: could be combined to a single Indirection pair
  std::vector<index_t> m_offset_to_index;
  std::vector<index_t> m_index_to_offset;
  // TODO: could store a singly linked list in index_to_offset
  std::vector<index_t> m_unused_index_to_offset;
};
