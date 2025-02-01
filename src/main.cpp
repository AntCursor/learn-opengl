// clang-format off
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
// clang-format on
#include "shader.h"
#include "texture.h"
#include "window.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);

constexpr int WIDTH{800};
constexpr int HEIGHT{600};

int main() {
  // clang-format off
  float vertices[] {
    // 1st triangle     // texture coords
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // top left 
  };
  unsigned int indices[] {
    // 1st triangle
    0, 1, 3,
    1, 2, 3,
  };
  // clang-format on

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  Window mainWindow{"Snake", WIDTH, HEIGHT};
  glfwMakeContextCurrent(mainWindow);

  glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << "\n";
    return -1;
  }

  glfwSetKeyCallback(mainWindow, key_callback);

  Shader shaderProgram{"src/vertex.glsl", "src/fragment.glsl"};
  Texture2D containerTex{"src/container.jpg"};
  Texture2D faceTex{"src/awesomeface.png"};

  unsigned int VAO{}, VBO{}, EBO{};
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  shaderProgram.use();
  int colorLoc = glGetUniformLocation(shaderProgram.id(), "uniColor");

  containerTex.bind(0);
  faceTex.bind(1);
  shaderProgram.setUInt("oTexture1", 0);
  shaderProgram.setUInt("oTexture2", 1);

  float mix{0.2f};
  shaderProgram.setUFloat("oMix", mix);

  int modelMatLoc{glGetUniformLocation(shaderProgram.id(), "modelMat")};
  int viewMatLoc{glGetUniformLocation(shaderProgram.id(), "viewMat")};
  int projectionMatLoc{
      glGetUniformLocation(shaderProgram.id(), "projectionMat")};

  glm::mat4 view{};
  view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -3.f));

  glm::mat4 projection{glm::perspective(
      glm::radians(60.f), mainWindow.getWidth() / (float)mainWindow.getHeight(),
      0.1f, 1000.f)};

  while (!glfwWindowShouldClose(mainWindow)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);

    glm::mat4 model{glm::mat4(1.f)};
    model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.f, 0.f, 0.f));
    glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(model));

    glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionMatLoc, 1, GL_FALSE,
                       glm::value_ptr(projection));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glUniform4f(colorLoc, 1.f, 1.f, 1.f, 1.0f);

    glfwSwapBuffers(mainWindow);
    glfwPollEvents();
  }
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  // This function exits with a segmentation fault on my pc for some reason.
  // glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, true);
  }
  if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
    int currentPolyMode[2]{};
    glGetIntegerv(GL_POLYGON_MODE, currentPolyMode);
    if (currentPolyMode[0] == GL_FILL) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
  }
}
