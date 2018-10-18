// SExp - A S-Expression Parser for C++
// Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <gtest/gtest.h>

#include <sstream>

#include "sexp/lexer.hpp"

TEST(LexerTest, simple_tokens)
{
  std::istringstream is("(foo . bar #())");
  sexp::Lexer lexer(is);
  ASSERT_EQ(sexp::Lexer::TOKEN_OPEN_PAREN, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.getNextToken());
  ASSERT_EQ("foo", lexer.getString());
  ASSERT_EQ(sexp::Lexer::TOKEN_DOT, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.getNextToken());
  ASSERT_EQ("bar", lexer.getString());
  ASSERT_EQ(sexp::Lexer::TOKEN_ARRAY_START, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_CLOSE_PAREN, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_CLOSE_PAREN, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_EOF, lexer.getNextToken());
}

TEST(LexerTest, long_tokens)
{
  std::string long_token(32768, 'X');
  std::istringstream is("(" + long_token + ")");
  sexp::Lexer lexer(is);
  ASSERT_EQ(sexp::Lexer::TOKEN_OPEN_PAREN, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.getNextToken());
  ASSERT_EQ(long_token, lexer.getString());
  ASSERT_EQ(sexp::Lexer::TOKEN_CLOSE_PAREN, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_EOF, lexer.getNextToken());
}

TEST(LexerTest, comment)
{
  std::istringstream is(";comment\n(foo ;comment\n;comment\n bar);EOF");
  sexp::Lexer lexer(is);
  ASSERT_EQ(sexp::Lexer::TOKEN_OPEN_PAREN, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.getNextToken());
  ASSERT_EQ(sexp::Lexer::TOKEN_CLOSE_PAREN, lexer.getNextToken());
}

TEST(LexerTest, token_dot)
{
  std::vector<std::string> texts = {
    "."
  };

  for(const auto& text : texts)
  {
    std::istringstream is(text);
    sexp::Lexer lexer(is);
    ASSERT_EQ(sexp::Lexer::TOKEN_DOT, lexer.getNextToken());
    ASSERT_EQ(text, lexer.getString());
  }
}

TEST(LexerTest, token_symbol)
{
  std::vector<std::string> texts = {
    "SymbolTest",
    "foo-bar",
    "1.2.3",
    "e50",
  };

  for(const auto& text : texts)
  {
    std::istringstream is(text);
    sexp::Lexer lexer(is);
    ASSERT_EQ(sexp::Lexer::TOKEN_SYMBOL, lexer.getNextToken());
    ASSERT_EQ(text, lexer.getString());
  }
}

TEST(LexerTest, token_string)
{
  std::istringstream is("\"StringTest\"");
  sexp::Lexer lexer(is);
  ASSERT_EQ(sexp::Lexer::TOKEN_STRING, lexer.getNextToken());
  ASSERT_EQ("StringTest", lexer.getString());
}

TEST(LexerTest, token_integer)
{
  std::vector<std::string> texts = {
    "123456789",
    "-123456789"
  };

  for(const auto& text : texts)
  {
    std::istringstream is(text);
    sexp::Lexer lexer(is);
    ASSERT_EQ(sexp::Lexer::TOKEN_INTEGER, lexer.getNextToken());
    ASSERT_EQ(text, lexer.getString());
  }
}

TEST(LexerTest, token_real)
{
  std::vector<std::string> texts = {
    ".1234",
    ".1234e15",
    "1234.6789",
    "1234.",
    "1234.5678",
    "1234.5678e15",
    "-1234.5678e15",
    "1234.5678e15",
  };

  for(const auto& text : texts)
  {
    std::istringstream is(text);
    sexp::Lexer lexer(is);
    ASSERT_EQ(sexp::Lexer::TOKEN_REAL, lexer.getNextToken());
    ASSERT_EQ(text, lexer.getString());
  }
}

/* EOF */
