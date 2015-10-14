#include <gtest/gtest.h>

#include <sstream>

#include "sexp/lexer.hpp"

TEST(LexerTest, simple_tokens)
{
  std::istringstream out("(foo . bar)");
  sexp::Lexer lexer(out);
  ASSERT_EQ(sexp::Lexer::TOKEN_OPEN_PAREN, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.getNextToken());
  ASSERT_EQ("foo", lexer.getString());
  ASSERT_EQ(sexp::Lexer::TOKEN_DOT, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.getNextToken());
  ASSERT_EQ("bar", lexer.getString());
  ASSERT_EQ(sexp::Lexer::TOKEN_CLOSE_PAREN, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_EOF, lexer.getNextToken());
}

TEST(LexerTest, long_tokens)
{
  std::string long_token(32768, 'X');
  std::istringstream out("(" + long_token + ")");
  sexp::Lexer lexer(out);
  ASSERT_EQ(sexp::Lexer::TOKEN_OPEN_PAREN, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.getNextToken());
  ASSERT_EQ(long_token, lexer.getString());
  ASSERT_EQ(sexp::Lexer::TOKEN_CLOSE_PAREN, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_EOF, lexer.getNextToken());
}

/* EOF */
