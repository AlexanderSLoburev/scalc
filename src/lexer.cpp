// lexer.cpp
#include "lexer.hpp"
#include "token.hpp"
#include <cctype>
#include <cstdio>
#include <stdexcept>
#include <string>

/**
 * Reads a single character from the input and increases the position counter.
 * If the end of the file is reached, returns EOF.
 * @return The character read from the input, or EOF if the end of the file is
 * reached.
 */
auto Lexer::_read_char() -> int {
  if (char chr = '\0'; _input.get(chr)) {
    ++_pos;
    return chr;
  }
  return EOF;
}

/**
 * Puts back a character to the input and decreases the position counter.
 * If the character cannot be put back, does nothing.
 * @param chr The character to be put back.
 */
auto Lexer::_putback_char(char chr) -> void {
  if (_input.putback(chr)) {
    --_pos;
  }
}

/**
 * Reads a string representing a number from the input and creates a token.
 * If the string does not represent a valid number, throws a std::domain_error.
 * @param first The first character of the number.
 * @return A Token representing the number, either Token::Kind::INTEGER or
 * Token::Kind::REAL.
 */
auto Lexer::_read_number(char first) -> Token {
  int chr = _input.peek();

  if (first == '.' &&
      (chr == EOF || !std::isdigit(static_cast<unsigned char>(chr)))) {
    // First check if the first character of the number is a dot (.) and if the
    // next character is either the end of the file (EOF) or not a digit. If
    // this condition is true, it throws a std::domain_error with a message
    // indicating a wrong number format.
    throw std::domain_error(std::string(MSG_WRONG_NUM_FMT) +
                            std::to_string(_pos));
  }

  std::string buffer;
  buffer.push_back(first);

  bool has_dot = (first == '.');
  bool has_exp = false;
  const unsigned START_POS = _pos;
  int next = 0;

  chr = _read_char();

  while (chr != EOF) {
    // Iterate through the input stream until the end of the file (EOF) is
    // reached or the first character that is not a part of the number is met.

    // Check the type of the current character:

    if (std::isdigit(static_cast<unsigned char>(chr))) {
      // If it's a digit, it appends the character to the buffer.
      buffer.push_back(static_cast<char>(chr));

    } else if (chr == '.') {
      // If it's a dot (.), it checks if has_dot or has_exp is true. If so, it
      // throws a std::domain_error with a message indicating a wrong number
      // format. Otherwise, it sets has_dot to true and appends the character to
      // the buffer.
      if (has_dot || has_exp) {
        _putback_char(static_cast<char>(chr));
        throw std::domain_error(std::string(MSG_WRONG_NUM_FMT) +
                                std::to_string(START_POS));
      }

      has_dot = true;
      buffer.push_back(static_cast<char>(chr));

    } else if ((chr == 'e' || chr == 'E')) {
      // If it's an exponent (e/E), it checks if has_exp is true. If so, it
      // throws a std::domain_error with a message indicating a wrong number
      // format. Otherwise, it sets has_exp to true and appends the character to
      // the buffer. It then checks the next character.
      if (has_exp) {
        _putback_char(static_cast<char>(chr));
        throw std::domain_error(std::string(MSG_WRONG_NUM_FMT) +
                                std::to_string(START_POS));
      }

      has_exp = true;
      buffer.push_back(static_cast<char>(chr));

      next = _input.peek();
      if (next == '+' || next == '-') {
        // If it's a + or - sign, it reads the next character and appends it to
        // the buffer.
        _read_char();
        buffer.push_back(static_cast<char>(next));
      }

      next = _input.peek();
      if (next == EOF || !std::isdigit(static_cast<unsigned char>(next))) {
        // If the next character is not a digit, it throws a std::domain_error
        // with a message indicating a wrong number format.
        throw std::domain_error(std::string(MSG_WRONG_NUM_FMT) +
                                std::to_string(START_POS));
      }

    } else {
      // If it's any other character, it puts the character back into the input
      // and breaks out of the loop.
      _putback_char(static_cast<char>(chr));
      break;
    }

    chr = _read_char();
  }

  // Return a Token object with the appropriate kind (Token::Kind::INTEGER or
  // Token::Kind::REAL) based on whether a dot or an exponent was encountered,
  // the buffer containing the number string, and the START_POS.
  return {(!has_dot && !has_exp) ? Token::Kind::INTEGER : Token::Kind::REAL,
          buffer, START_POS};
}

/**
 * Reads an operator from the input and returns a Token object with the
 * appropriate kind.
 * If the operator is not recognized, throws a std::domain_error with a
 * message indicating a wrong operator at the given position.
 * @param first The first character of the operator.
 * @return A Token object with the appropriate kind and the START_POS.
 */
auto Lexer::_read_operator(char first) -> Token {
  const unsigned START_POS = _pos;
  const int NEXT = _input.peek();

  switch (first) {
    // The switch statement checks the value of first and returns a Token object
    // with the corresponding kind for each case.

    // If first is one of the
    // following characters: '+', '-', '*', '/', '%', '!', '(', ')', '{', '}',
    // '^', '&', '|', '~', it returns a Token object with the corresponding
    // kind.
  case '+':
    return {Token::Kind::PLUS, "", START_POS};
  case '-':
    return {Token::Kind::MINUS, "", START_POS};
  case '*':
    return {Token::Kind::MULT, "", START_POS};
  case '/':
    return {Token::Kind::DIV, "", START_POS};
  case '%':
    return {Token::Kind::MOD, "", START_POS};
  case '!':
    return {Token::Kind::FACTORIAL, "", START_POS};
  case '(':
    return {Token::Kind::L_PAREN, "", START_POS};
  case ')':
    return {Token::Kind::R_PAREN, "", START_POS};
  case '{':
    return {Token::Kind::L_BRACE, "", START_POS};
  case '}':
    return {Token::Kind::R_BRACE, "", START_POS};
  case '^':
    return {Token::Kind::BIT_XOR, "", START_POS};
  case '&':
    return {Token::Kind::BIT_AND, "", START_POS};
  case '|':
    return {Token::Kind::BIT_OR, "", START_POS};
  case '~':
    return {Token::Kind::BIT_NOT, "", START_POS};
  case '<':
    // If first is '<' and the next character is also '<', it reads the next
    // character and returns a Token object with the kind
    // Token::Kind::BIT_L_SHIFT.
    if (NEXT == '<') {
      _read_char();
      return {Token::Kind::BIT_L_SHIFT, "", START_POS};
    } else {
      // If first is '<' and the next character is not also '<', it throws a
      // std::domain_error with a message indicating a wrong operator at the
      // given position.
      throw std::domain_error(std::string(MSG_WRONG_OPERATOR) +
                              std::to_string(START_POS));
    }
  case '>':
    if (NEXT == '>') {
      // If first is '>' and the next character is also '>', it reads the next
      // character and returns a Token object with the kind
      // Token::Kind::BIT_R_SHIFT.
      _read_char();
      return {Token::Kind::BIT_R_SHIFT, "", START_POS};
    } else {
      // If first is '>' and the next character is not also '>', it throws a
      // std::domain_error with a message indicating a wrong operator at the
      // given position.
      throw std::domain_error(std::string(MSG_WRONG_OPERATOR) +
                              std::to_string(START_POS));
    }
  default:
    // If first is not recognized as a valid operator, it throws a
    // std::domain_error with a message indicating a wrong operator at the
    // given position.
    throw std::domain_error(std::string(MSG_WRONG_OPERATOR) +
                            std::to_string(START_POS));
  }
}

auto Lexer::next() -> Token {
  int chr = _read_char();

  while (chr != EOF) {
    if (chr == '\n') {
      return {Token::Kind::END_OF_EXPR, "", _pos};
    }

    if (chr == '#') {
      return {Token::Kind::END, "", _pos};
    }

    if (std::isspace(static_cast<unsigned char>(chr))) {
      chr = _read_char();
      continue;
    }

    if (std::isdigit(static_cast<unsigned char>(chr)) || chr == '.') {
      return _read_number(static_cast<char>(chr));
    }

    return _read_operator(static_cast<char>(chr));
  }

  return {Token::Kind::END, "", _pos};
}