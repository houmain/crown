#pragma once

#include "platform/platform.h"
#include "Resource.h"
#include <vector>
#include <string>

class ShaderProgram {
public:
  ShaderProgram() = default;
  ShaderProgram(std::string_view vs_source, std::string_view fs_source,
    const std::vector<const char*>& attributes);
  void bind();
  operator GLuint() const { return m_program; }

private:
  Resource<GLuint> m_program;
};
