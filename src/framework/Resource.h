#pragma once

#include <utility>

template <typename Handle,
          typename Free = void(*)(Handle value),
          bool free_null = false>
class Resource {
public:
  Resource() = default;
  Resource(Handle handle, Free free) noexcept
    : m_handle(std::move(handle)), m_free(free) {
  }
  Resource(Resource&& rhs) noexcept {
    std::swap(m_handle, rhs.m_handle);
    std::swap(m_free, rhs.m_free);
  }
  Resource& operator=(Resource&& rhs) noexcept {
    auto tmp = std::move(rhs);
    std::swap(m_handle, tmp.m_handle);
    std::swap(m_free, tmp.m_free);
    return *this;
  }
  ~Resource() {
    reset();
  }
  explicit operator bool() const { return static_cast<bool>(m_handle); }
  operator const Handle&() const { return m_handle; }
  decltype(auto) operator->() const { return m_handle; }

  void reset() {
    if (m_free) {
      if (free_null || *this)
        m_free(std::move(m_handle));
      m_handle = { };
      m_free = nullptr;
    }
  }
  Handle release() {
    auto tmp = m_handle;
    m_handle = { };
    m_free = nullptr;
    return tmp;
  }

private:
  Handle m_handle{ };
  Free m_free{ };
};
