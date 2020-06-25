
// tweakable.h - v1.0  - public domain
// no warranty implied; use at your own risk
//
// Tweakable is a C++11 header-file library, which povides the macro TWEAKABLE.
// Putting a constant in the macro, allows the variable to be tweaked while the
// application is running, just by modifying (and saving) the source. e.g.
//
//   float x = TWEAKABLE(55.0f);
//   float y = TWEAKABLE(130.0f);
//   auto color = TWEAKABLE(0x336699);
//   draw_string(x, y, color, TWEAKABLE("this is a test"));
//
// Just like assert.h, the library is deactivated by default in release builds.
// If it is deactivated, TWEAKABLE is simply replaced with its arguments. To
// override the default, TWEAKABLE_ENABLED or TWEAKABLE_DISABLED can be defined.
//
// Exactly one .cpp file should define TWEAKABLE_IMPLEMENTATION, before it
// includes this header, so the library is compiled and linked only once.
//
// The application only uses the information that is available at compile time,
// namely __FILE__ and __LINE__, to detect and parse the changes of the source.
// Therefore it can only find the corresponding location if:
//   - the source is available at the path the compiler replaced __FILE__ with.
//   - the line numbers still match.
//   - there is no more than one TWEAKABLE per line.
//
// TWEAKABLE is overloaded, so a type or function, together with a set of
// arguments, can be passed to initialize the value. e.g.
//
//   struct vec3 {
//     vec3(float x, float y, float z) ...
//   };
//   set_position(TWEAKABLE(vec3, 0.0f, 8.0f, 0.0f));
//
//   uint32_t make_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) ...
//   fill_rect(x, y, w, h, TWEAKABLE(make_rgba, 100, 200, 250, 255));
//
// The result of TWEAKABLE is of an unspecified type, which implicitly converts
// to the type of the passed argument. Global constants should be passed to
// auto, to preserve the special behavior. e.g.
//
//   const auto g_background_color = TWEAKABLE(0x336699);
//
// The file modification time it checked every time a value is accessed. If the
// time did not change, the file is not scanned again. The cost, of accessing a
// value, is comparable to a string to number conversion (atoi, atof, ...).

#pragma once

#if !defined(TWEAKABLE_DISABLED) && !defined(TWEAKABLE_ENABLED)
#  if defined(NDEBUG)
#    define TWEAKABLE_DISABLED
#  else 
#    define TWEAKABLE_ENABLED
#  endif
#endif

#if defined(TWEAKABLE_DISABLED)

#define _TWEAKABLE_0(A0) A0
#define _TWEAKABLE_1(CTOR, A0) (CTOR)(A0)
#define _TWEAKABLE_2(CTOR, A0, A1) (CTOR)(A0, A1)
#define _TWEAKABLE_3(CTOR, A0, A1, A2) (CTOR)(A0, A1, A2)
#define _TWEAKABLE_4(CTOR, A0, A1, A2, A3) (CTOR)(A0, A1, A2, A3)
#define _TWEAKABLE_N(a, b, c, d, e, NAME, ...) NAME
#define TWEAKABLE(...) _TWEAKABLE_N(__VA_ARGS__, \
    _TWEAKABLE_4, _TWEAKABLE_3, _TWEAKABLE_2, _TWEAKABLE_1, _TWEAKABLE_0)(__VA_ARGS__)

#else // TWEAKABLE_ENABLED

#define _TWEAKABLE_0(A0) \
  _tweakable::make_var([&]() { \
      auto it = _tweakable::get_arguments(__FILE__, __LINE__); \
      return _tweakable::read_value(&it, A0); \
    })

#define _TWEAKABLE_1(CTOR, A0) \
  _tweakable::make_var([&]() { \
      auto it = _tweakable::get_arguments(__FILE__, __LINE__); \
      auto a0 = _tweakable::read_value(&it, A0); \
      return (CTOR)(a0); \
    })

#define _TWEAKABLE_2(CTOR, A0, A1) \
  _tweakable::make_var([&]() { \
      auto it = _tweakable::get_arguments(__FILE__, __LINE__); \
      auto a0 = _tweakable::read_value(&it, A0); \
      auto a1 = _tweakable::read_value(&it, A1); \
      return (CTOR)(a0, a1); \
    })

#define _TWEAKABLE_3(CTOR, A0, A1, A2) \
  _tweakable::make_var([&]() { \
      auto it = _tweakable::get_arguments(__FILE__, __LINE__); \
      auto a0 = _tweakable::read_value(&it, A0); \
      auto a1 = _tweakable::read_value(&it, A1); \
      auto a2 = _tweakable::read_value(&it, A2); \
      return (CTOR)(a0, a1, a2); \
    })

#define _TWEAKABLE_4(CTOR, A0, A1, A2, A3) \
  _tweakable::make_var([&]() { \
      auto it = _tweakable::get_arguments(__FILE__, __LINE__); \
      auto a0 = _tweakable::read_value(&it, A0); \
      auto a1 = _tweakable::read_value(&it, A1); \
      auto a2 = _tweakable::read_value(&it, A2); \
      auto a3 = _tweakable::read_value(&it, A3); \
      return (CTOR)(a0, a1, a2, a3); \
    })

#define _TWEAKABLE_N(a, b, c, d, e, NAME, ...) NAME

#define TWEAKABLE(...) _TWEAKABLE_N(__VA_ARGS__, \
    _TWEAKABLE_4, _TWEAKABLE_3, _TWEAKABLE_2, _TWEAKABLE_1, _TWEAKABLE_0)(__VA_ARGS__)

namespace _tweakable {
  const char* get_arguments(const char* filename, int line);
  bool read_value(const char** it, bool default_value);
  int read_value(const char** it, int default_value);
  float read_value(const char** it, float default_value);
  double read_value(const char** it, double default_value);
  const char* read_value(const char** it, const char* default_value);

  template <typename F>
  class Var {
  public:
    Var(F read_value)
      : m_read_value(read_value) {
    }
    using T = decltype( (*((F*)nullptr))() );
    operator T() const {
      return m_read_value();
    }
  private:
    F m_read_value;
  };

  template <typename F>
  Var<F> make_var(F read_value) {
    return { read_value };
  }
} // namespace

#if defined(TWEAKABLE_IMPLEMENTATION)

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include "tweakable.h"

#if defined(__linux)
#  include <sys/stat.h>
#elif defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#else
#  error platform not implemented
#endif

namespace {

struct FileInfo {
  const char* filename;
#if defined(__linux)
  timespec modification_time;
#elif defined(_WIN32)
  FILETIME modification_time;
#endif
  std::string buffer;
  // std::string::npos means no tweakable in line
  std::vector<size_t> line_arguments;

  FileInfo(const char* filename)
    : filename(filename) {
  }
};

bool check_modification(FileInfo& file) {
#if defined(__linux)
  struct stat s;
  if (stat(file.filename, &s) >= 0 &&
      (s.st_mtim.tv_sec != file.modification_time.tv_sec ||
        s.st_mtim.tv_nsec != file.modification_time.tv_nsec)) {
    file.modification_time = s.st_mtim;
    return true;
  }
#elif defined(_WIN32)
  auto handle = CreateFileA(file.filename, GENERIC_READ | FILE_WRITE_ATTRIBUTES,
    0, NULL, OPEN_EXISTING, 0, NULL);
  if (handle != INVALID_HANDLE_VALUE) {
    FILETIME modification_time;
    if (GetFileTime(handle, NULL, NULL, &modification_time) != 0 &&
        (modification_time.dwHighDateTime != file.modification_time.dwHighDateTime ||
          modification_time.dwLowDateTime != file.modification_time.dwLowDateTime)) {
      file.modification_time = modification_time;
      CloseHandle(handle);
      return true;
    }
    CloseHandle(handle);
  }
#endif
  return false;
}

void reload_lines(FileInfo& file) {
  file.buffer.clear();
  file.line_arguments.clear();

  auto handle = std::fopen(file.filename, "r");
  struct Guard {
    FILE* handle;

    ~Guard() {
      if (handle)
        std::fclose(handle);
    }
  } guard { handle };

  char line[1024];
  while (std::fgets(line, sizeof(line), handle)) {
    // ignore beginning of unreasonably long line
    if (!std::strchr(line, '\n'))
      continue;

    if (auto it = std::strstr(line, "TWEAKABLE("))
      if (const auto end = std::strstr(it + 10, ")")) {
        it += 10;

        // if there is more than one argument, then skip the first
        const auto ctor = std::strstr(it, ",");
        if (ctor && ctor < end)
          it = ctor + 1;

        // current position of buffer is beginning of line's argument list
        file.line_arguments.push_back(file.buffer.size());

        for (; it != end; ++it) {
          const auto c = *it;
          if (std::isspace(c))
            continue;

          if (c == ',') {
            // end of argument
            file.buffer += '\0';
          }
          else if (c == '"') {
            // string argument
            if (auto string_end = std::strchr(it + 1, '"')) {
              file.buffer.append(it + 1, string_end);
              it = string_end;
            }
          }
          else {
            file.buffer += c;
          }
        }
        // end of argument list
        file.buffer += '\0';
        continue;
      }
    // line has no arguments
    file.line_arguments.push_back(std::string::npos);
  }
}

const char* advance(const char** it) {
  auto begin = *it;
  // skip past argument delimiter
  *it = std::strchr(*it, '\0') + 1;
  return begin;
}

class FileCache {
public:
  static FileCache& instance() {
    static FileCache s_instance;
    return s_instance;
  }

  const char* get_arguments(const char* filename, int line) {
    // binary search for file
    auto it = std::lower_bound(begin(m_files), end(m_files), filename,
      [](const FileInfo& file, const char* filename) {
        return file.filename < filename;
      });
    if (it == end(m_files) || it->filename != filename)
      it = m_files.emplace(it, filename);

    if (check_modification(*it))
      reload_lines(*it);

    if (line < static_cast<int>(it->line_arguments.size()))
      if (it->line_arguments[line] != std::string::npos)
        return it->buffer.c_str() + it->line_arguments[line];

    return nullptr;
  }

private:
  std::vector<FileInfo> m_files;
};

} // namespace

namespace _tweakable {

const char* get_arguments(const char* filename, int line) {
  return FileCache::instance().get_arguments(filename, line - 1);
}

int read_value(const char** it, int default_value) {
  if (*it) {
    auto begin = advance(it);
    auto base = 10;
    if (*begin == '0') {
      base = 8;
      ++begin;
      if (std::tolower(*begin) == 'x') {
        base = 16;
        ++begin;
      }
      else if (std::tolower(*begin) == 'b') {
        base = 2;
        ++begin;
      }
    }
    return std::strtol(begin, nullptr, base);
  }
  return default_value;
}

bool read_value(const char** it, bool default_value) {
  if (*it) {
    const auto begin = advance(it);
    return (std::strncmp(begin, "false", 5) != 0 &&
            std::strncmp(begin, "0", 1) != 0);
  }
  return default_value;
}

float read_value(const char** it, float default_value) {
  if (*it)
    return static_cast<float>(std::atof(advance(it)));
  return default_value;
}

double read_value(const char** it, double default_value) {
  if (*it)
    return std::atof(advance(it));
  return default_value;
}

const char* read_value(const char** it, const char* default_value) {
  if (*it)
    return advance(it);
  return default_value;
}

} // namespace

#endif // TWEAKABLE_IMPLEMENTATION
#endif // TWEAKABLE_ENABLED
