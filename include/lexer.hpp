// lexer.hpp
#ifndef LEXER_HPP
#define LEXER_HPP
#include "token.hpp"
#include <istream>
#include <string_view>

class Lexer {
public:
  explicit Lexer(std::istream &ins) : _input(ins) {}
  // Read the next token from the input.
  auto next() -> Token;

private:
  std::istream
      &_input; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
  Token _buffer;
  // The current position in the input stream. 0 means before the first
  // character.
  unsigned _pos = 0;

  static constexpr std::string_view MSG_WRONG_NUM_FMT =
      "wrong number format at position ";
  static constexpr std::string_view MSG_WRONG_OPERATOR =
      "wrong operator at position ";

  // Read a single character from the input and increase the position counter.
  auto _read_char() -> int;
  // Put back a character to the input and decrease the position counter.
  auto _putback_char(char chr) -> void;
  // Read a string representing a number from the input and create a token.
  auto _read_number(char first) -> Token;
  // Read a string representing an operator from the input and create a token.
  auto _read_operator(char first) -> Token;
};

#endif // LEXER_HPP