#ifndef SHADER_CLASS_LEARNOGL_H
#define SHADER_CLASS_LEARNOGL_H

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
  unsigned int m_id{};

 public:
  Shader(const char* vertexSourcePath, const char* fragmentSourcePath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      // open files
      vShaderFile.open(vertexSourcePath);
      fShaderFile.open(fragmentSourcePath);
      std::stringstream vShaderStream, fShaderStream;
      // read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      // close file handlers
      vShaderFile.close();
      fShaderFile.close();
      // convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex{glCreateShader(GL_VERTEX_SHADER)};
    unsigned int fragment{glCreateShader(GL_FRAGMENT_SHADER)};

    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);

    glCompileShader(vertex);
    shaderCompSuccess(vertex);
    glCompileShader(fragment);
    shaderCompSuccess(fragment);

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);

    glLinkProgram(m_id);
    shaderLinkSuccess();

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  ~Shader() { glDeleteProgram(m_id); }

  void use() { glUseProgram(m_id); };
  unsigned int id() { return m_id; }
  void setUInt(const char* uniform, int value) {
    glUniform1i(glGetUniformLocation(m_id, uniform), value);
  }
  void setUFloat(const char* uniform, float value) {
    glUniform1f(glGetUniformLocation(m_id, uniform), value);
  }

 private:
  bool shaderCompSuccess(unsigned int shader) {
    int success{};
    char infoLog[512]{};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      std::cerr << "Shader compilation error: " << infoLog;
      return false;
    }
    return true;
  }
  bool shaderLinkSuccess() {
    int success{};
    char infoLog[512]{};
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);

    if (!success) {
      glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
      std::cerr << "Shader linking error: " << infoLog;
      return false;
    }
    return true;
  }
};

#endif  // !SHADER_CLASS_LEARNOGL_H
