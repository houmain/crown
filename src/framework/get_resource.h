#pragma once

#include <string>

std::pair<const void*, size_t> get_resource(std::string_view filename);
