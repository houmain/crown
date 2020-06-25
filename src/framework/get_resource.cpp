
#include "platform/platform.h"

#if 0 && !defined(NDEBUG)
#include <vector>
#include <map>

std::pair<const void*, size_t> get_resource(std::string_view filename) {
  const auto resources_path = "./resources/";

  static auto s_data =  std::map<std::string, std::vector<uint8_t>>();
  auto& data = s_data[std::string(filename)];
  auto file = std::fopen((resources_path + std::string(filename)).c_str(), "rb");
  if (!file)
    platform_error("loading resource failed: " + std::string(filename));

  fseek(file, 0, SEEK_END);
  data.resize(ftell(file));
  fseek(file, 0, SEEK_SET);
  fread(data.data(), data.size(), 1, file);
  fclose(file);
  return { data.data(), data.size() };
}

#else // defined(NDEBUG)

std::pair<const void*, size_t> get_resource(std::string_view filename) {
# define RESOURCE_BEGIN(NAME) \
  if (filename == NAME) { \
    static const uint8_t s_data[] = {
# define RESOURCE_END }; \
    return { s_data, sizeof(s_data) }; \
  }
# include "resources/resources.inc"
  platform_error(("loading resource failed: " + std::string(filename)).c_str());
  return { };
}

#endif // defined(NDEBUG)
