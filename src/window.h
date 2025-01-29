#ifndef WINDOW_LEARNOGL_H
#define WINDOW_LEARNOGL_H

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <string>

class Window {
  GLFWwindow *m_window{};
  int m_width{};
  int m_height{};
  std::string m_title{};

  int m_xViewPos{};
  int m_yViewPos{};

public:
  Window(const char *title, int window_width = 800, int window_height = 600)
      : m_width{window_width}, m_height{window_height}, m_title{title} {
    m_window =
        glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    assert(m_window != nullptr && "Failed to create window");
  }
  constexpr int getWidth() const { return m_width; }
  constexpr int getHeight() const { return m_height; }
  const std::string &getTitle() const { return m_title; }

  constexpr GLFWwindow *getWindow() const { return m_window; }
  constexpr operator GLFWwindow *() const { return m_window; }

  void use() {
    glfwMakeContextCurrent(m_window);
    glViewport(m_xViewPos, m_yViewPos, m_width, m_height);
  }
};

#endif // !WINDOW_LEARNOGL_H
