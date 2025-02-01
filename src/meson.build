source_files = ['main.cpp', 'glad.c', 'window.h', 'shader.h', 'texture.h']
glfw_dep = dependency('glfw3')
deps = [glfw_dep]
inc = ['include']
cpp_arg = []
link_arg = [ '-lX11', '-lXi', '-lXrandr', '-lpthread', '-lGL', '-ldl' ]

executable(
  'logl',
  source_files,
  dependencies: deps,
  include_directories: inc,
  cpp_args: cpp_arg,
  link_args: link_arg
)
