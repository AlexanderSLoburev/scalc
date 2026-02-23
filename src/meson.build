# src/meson.build

lexer_lib = static_library(
  'lexer',
  [
    'lexer.cpp',
  ],
  include_directories: inc
)

lexer_dep = declare_dependency(
  link_with: lexer_lib,
  include_directories: inc
)

executable(
  'scalc',
  ['scalc.cpp'],
  link_with: lexer_lib,
  include_directories: inc
)
