#ifndef TEXTURE_LOGL_CLASS_H
#define TEXTURE_LOGL_CLASS_H

#include <glad/glad.h>

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdexcept>

class Texture2D {
  unsigned char* m_data{};
  unsigned int m_id{};
  int m_width{};
  int m_height{};

 public:
  Texture2D(const char* filePath, int wrapMode = GL_REPEAT,
            int filterMode = GL_LINEAR, int mipmapFilter = GL_NEAREST) {
    int _channels{};
    stbi_set_flip_vertically_on_load(true);
    m_data = stbi_load(filePath, &m_width, &m_height, &_channels, 3);
    if (!m_data) throw std::runtime_error("Failed to load texture.");

    switch ((filterMode + 1) ^ mipmapFilter) {
      case (GL_LINEAR + 1) ^ GL_LINEAR:
        mipmapFilter = GL_LINEAR_MIPMAP_LINEAR;
        break;
      case (GL_LINEAR + 1) ^ GL_NEAREST:
        mipmapFilter = GL_LINEAR_MIPMAP_NEAREST;
        break;
      case (GL_NEAREST + 1) ^ GL_LINEAR:
        mipmapFilter = GL_NEAREST_MIPMAP_LINEAR;
        break;
      case (GL_NEAREST + 1) ^ GL_NEAREST:
        mipmapFilter = GL_NEAREST_MIPMAP_NEAREST;
        break;
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmapFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, m_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(m_data);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  ~Texture2D() { glDeleteTextures(1, &m_id); }

  void bind(int activeTex) {
    glActiveTexture(GL_TEXTURE0 + activeTex);
    glBindTexture(GL_TEXTURE_2D, m_id);
  }
};

#endif  // !TEXTURE_LOGL_CLASS_H
