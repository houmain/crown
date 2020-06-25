
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(std::string_view vs_source,
    std::string_view fs_source, const std::vector<const char*>& attributes) {
  m_program = { glCreateProgram(), glDeleteProgram };

  auto vs = glCreateShader(GL_VERTEX_SHADER);
  auto source = vs_source.data();
  auto length = static_cast<GLint>( vs_source.size());
  glShaderSource(vs, 1, &source, &length);
  glCompileShader(vs);

  auto result = GLint{ };
  glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
  if (!result)
    platform_error("compiling vertex shader failed");

  glAttachShader(m_program, vs);
  glDeleteShader(vs);

  auto fs = glCreateShader(GL_FRAGMENT_SHADER);
  source = fs_source.data();
  length = static_cast<GLint>( fs_source.size());
  glShaderSource(fs, 1, &source, &length);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
  if (!result)
    platform_error("compiling fragment shader failed");

  glAttachShader(m_program, fs);
  glDeleteShader(fs);

  for (size_t i = 0; i < attributes.size(); ++i)
    glBindAttribLocation(m_program, i, attributes[i]);

  glLinkProgram(m_program);
  glGetProgramiv(m_program, GL_LINK_STATUS, &result);
  if (!result)
    platform_error("linking shader program failed");
}

void ShaderProgram::bind() {
  glUseProgram(m_program);
}
