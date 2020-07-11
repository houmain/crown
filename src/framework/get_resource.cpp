
#include "platform/platform.h"

std::pair<const void*, size_t> get_resource(std::string_view filename) {
# define RESOURCE_BEGIN(NAME) \
  if (filename == NAME) { \
    static const uint8_t s_data[] = {
# define RESOURCE_END }; \
    return { s_data, sizeof(s_data) }; \
  }
# include "resources/_generated/resources.inc"
  platform_error(("loading resource failed: " + std::string(filename)).c_str());
  return { };
}
