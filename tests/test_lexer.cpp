// tests/test_lexer.cpp

#include "lexer.hpp"
#include "token.hpp"
#include <gtest/gtest.h>
#include <sstream>

TEST(LexerTests, ParsesIntegers) {
  std::istringstream input("123 456");
  Lexer lex(input);

  Token t1 = lex.next();
  EXPECT_EQ(t1.kind(), Token::Kind::INTEGER);
  EXPECT_EQ(t1.value(), "123");

  Token t2 = lex.next();
  EXPECT_EQ(t2.kind(), Token::Kind::INTEGER);
  EXPECT_EQ(t2.value(), "456");
}

TEST(LexerTests, ParsesRealNumbers) {
  std::istringstream input("3.14 1e10 .5 2.");
  Lexer lex(input);

  Token t1 = lex.next();
  EXPECT_EQ(t1.kind(), Token::Kind::REAL);
  EXPECT_EQ(t1.value(), "3.14");

  Token t2 = lex.next();
  EXPECT_EQ(t2.kind(), Token::Kind::REAL);
  EXPECT_EQ(t2.value(), "1e10");

  Token t3 = lex.next();
  EXPECT_EQ(t3.kind(), Token::Kind::REAL);
  EXPECT_EQ(t3.value(), ".5");

  Token t4 = lex.next();
  EXPECT_EQ(t4.kind(), Token::Kind::REAL);
  EXPECT_EQ(t4.value(), "2.");
}

TEST(LexerTests, ParsesOperators) {
  std::istringstream input("+ - * / % !");
  Lexer lex(input);

  EXPECT_EQ(lex.next().kind(), Token::Kind::PLUS);
  EXPECT_EQ(lex.next().kind(), Token::Kind::MINUS);
  EXPECT_EQ(lex.next().kind(), Token::Kind::MULT);
  EXPECT_EQ(lex.next().kind(), Token::Kind::DIV);
  EXPECT_EQ(lex.next().kind(), Token::Kind::MOD);
  EXPECT_EQ(lex.next().kind(), Token::Kind::FACTORIAL);
}

TEST(LexerTests, ParsesParenthesesAndBraces) {
  std::istringstream input("(){}");
  Lexer lex(input);

  EXPECT_EQ(lex.next().kind(), Token::Kind::L_PAREN);
  EXPECT_EQ(lex.next().kind(), Token::Kind::R_PAREN);
  EXPECT_EQ(lex.next().kind(), Token::Kind::L_BRACE);
  EXPECT_EQ(lex.next().kind(), Token::Kind::R_BRACE);
}

TEST(LexerTests, ParsesBitwiseOperators) {
  std::istringstream input("~ & | ^ << >>");
  Lexer lex(input);

  EXPECT_EQ(lex.next().kind(), Token::Kind::BIT_NOT);
  EXPECT_EQ(lex.next().kind(), Token::Kind::BIT_AND);
  EXPECT_EQ(lex.next().kind(), Token::Kind::BIT_OR);
  EXPECT_EQ(lex.next().kind(), Token::Kind::BIT_XOR);
  EXPECT_EQ(lex.next().kind(), Token::Kind::BIT_L_SHIFT);
  EXPECT_EQ(lex.next().kind(), Token::Kind::BIT_R_SHIFT);
}

TEST(LexerTests, SkipsWhitespace) {
  std::istringstream input("   42   +   5   ");
  Lexer lex(input);

  EXPECT_EQ(lex.next().value(), "42");
  EXPECT_EQ(lex.next().kind(), Token::Kind::PLUS);
  EXPECT_EQ(lex.next().value(), "5");
}

TEST(LexerTests, HandlesEndOfExprAndEnd) {
  std::istringstream input("1 + 2\n3 + 4#");
  Lexer lex(input);

  EXPECT_EQ(lex.next().value(), "1");
  EXPECT_EQ(lex.next().kind(), Token::Kind::PLUS);
  EXPECT_EQ(lex.next().value(), "2");

  EXPECT_EQ(lex.next().kind(), Token::Kind::END_OF_EXPR);

  EXPECT_EQ(lex.next().value(), "3");
  EXPECT_EQ(lex.next().kind(), Token::Kind::PLUS);
  EXPECT_EQ(lex.next().value(), "4");

  EXPECT_EQ(lex.next().kind(), Token::Kind::END);
}

TEST(LexerTests, ThrowsOnInvalidNumberFormat) {
  std::istringstream input("1..2");
  Lexer lex(input);

  EXPECT_THROW(lex.next(), std::domain_error);
}

TEST(LexerTests, TracksTokenPositions) {
  std::istringstream input("12 + 34");
  Lexer lex(input);

  Token t1 = lex.next();
  EXPECT_EQ(t1.pos(), 1);

  Token t2 = lex.next();
  EXPECT_EQ(t2.pos(), 4);

  Token t3 = lex.next();
  EXPECT_EQ(t3.pos(), 6);
}
