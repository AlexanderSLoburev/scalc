// token.hpp
#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <cstdint>
#include <string>
#include <utility>

class Token {
public:
  enum class Kind : std::uint8_t {
    UNDEFINED = 0,
    INTEGER,
    REAL,
    END_OF_EXPR,
    END,
    L_PAREN,
    R_PAREN,
    L_BRACE,
    R_BRACE,
    PLUS,
    MINUS,
    MULT,
    DIV,
    FACTORIAL,
    MOD,
    BIT_NOT,
    BIT_L_SHIFT,
    BIT_R_SHIFT,
    BIT_AND,
    BIT_XOR,
    BIT_OR,
  };

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  Token(Kind kind = Kind::UNDEFINED, std::string value = "", unsigned pos = 0)
      : _kind(kind), _value(std::move(value)), _pos(pos) {
    ;
  }
  [[nodiscard]] auto kind() const -> Kind { return _kind; }
  [[nodiscard]] auto value() const -> std::string { return _value; }
  [[nodiscard]] auto pos() const -> unsigned { return _pos; }

private:
  Kind _kind;
  std::string _value;
  unsigned _pos;
};

#endif // TOKEN_HPP