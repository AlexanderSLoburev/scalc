# tests/meson.build

gtest = dependency('gtest')
gtest_main = dependency('gtest_main')

test_exe = executable(
  'lexer_tests',
  ['test_lexer.cpp'],
  dependencies: [lexer_dep, gtest, gtest_main],
)

test('lexer tests', test_exe)
