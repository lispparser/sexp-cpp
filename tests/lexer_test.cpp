#include <gtest/gtest.h>

#include <sstream>

#include "lisp/lexer.hpp"

TEST(LexerTest, simple_tokens)
{
  std::istringstream out("(foo . bar)");
  lisp::Lexer lexer(out);
  ASSERT_EQ(lisp::Lexer::TOKEN_OPEN_PAREN, lexer.getNextToken());
  ASSERT_EQ(lisp::Lexer::TOKEN_SYMBOL, lexer.getNextToken());
  ASSERT_EQ("foo", lexer.getString());
  ASSERT_EQ(lisp::Lexer::TOKEN_DOT, lexer.getNextToken());
  ASSERT_EQ(lisp::Lexer::TOKEN_SYMBOL, lexer.getNextToken());
  ASSERT_EQ("bar", lexer.getString());
  ASSERT_EQ(lisp::Lexer::TOKEN_CLOSE_PAREN, lexer.getNextToken());
  ASSERT_EQ(lisp::Lexer::TOKEN_EOF, lexer.getNextToken());
}

TEST(LexerTest, long_tokens)
{
  std::string long_token(32768, 'X');
  std::istringstream out("(" + long_token + ")");
  lisp::Lexer lexer(out);
  ASSERT_EQ(lisp::Lexer::TOKEN_OPEN_PAREN, lexer.getNextToken());
  ASSERT_EQ(lisp::Lexer::TOKEN_SYMBOL, lexer.getNextToken());
  ASSERT_EQ(long_token, lexer.getString());
  ASSERT_EQ(lisp::Lexer::TOKEN_CLOSE_PAREN, lexer.getNextToken());
  ASSERT_EQ(lisp::Lexer::TOKEN_EOF, lexer.getNextToken());
}

/* EOF */
