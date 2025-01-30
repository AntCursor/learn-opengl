source_files = ['main.cpp', 'glad.c', 'window.h', 'shader.h']
glfw_dep = dependency('glfw3')
deps = [glfw_dep]
inc = ['include']
cpp_arg = []
link_arg = []

executable(
  'logl',
  source_files,
  dependencies: deps,
  include_directories: inc,
  cpp_args: cpp_arg,
  link_args: link_arg
)
