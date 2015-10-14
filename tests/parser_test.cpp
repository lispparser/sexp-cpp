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

#include "sexp/io.hpp"
#include "sexp/value.hpp"
#include "sexp/parser.hpp"

TEST(ParserTest, simple)
{
  std::istringstream in("(1 2.5 foo \"TEXT\" bar bar)");
  sexp::Value cons = sexp::Parser::from_stream(in);
}

TEST(ParserTest, parse_integer)
{
  auto sexpr = sexp::Parser::from_string("12345");
  ASSERT_EQ(12345, sexpr.as_int());
}

TEST(ParserTest, parse_real)
{
  auto sexpr = sexp::Parser::from_string("0.125");
  ASSERT_EQ(sexp::Value::TYPE_REAL, sexpr.get_type());
  ASSERT_EQ(0.125f, sexpr.as_float());

  sexpr = sexp::Parser::from_string(".125");
  ASSERT_EQ(sexp::Value::TYPE_REAL, sexpr.get_type());
  ASSERT_EQ(0.125f, sexpr.as_float());
}

TEST(ParserTest, parse_string)
{
  auto sexpr = sexp::Parser::from_string("\"Hello\\nWorld\"");
  ASSERT_EQ(sexp::Value::TYPE_STRING, sexpr.get_type());
  ASSERT_EQ("Hello\nWorld", sexpr.as_string());
}

TEST(ParserTest, parse_symbol)
{
  auto sexpr = sexp::Parser::from_string("HelloWorld");
  ASSERT_EQ(sexp::Value::TYPE_SYMBOL, sexpr.get_type());
  ASSERT_EQ("HelloWorld", sexpr.as_string());
}

// FIXME: Compare data structure or use simple strings?!
// "(foo . bar)" as string is ambigous in the current parser as . can be handled as symbol, not pair
TEST(ParserTest, simple_pair)
{
  sexp::Value sexpr = sexp::Parser::from_string("(foo . bar)");
  ASSERT_EQ("(foo . bar)", sexpr.str());
  ASSERT_EQ("foo", sexpr.get_car().as_string());
  ASSERT_EQ("bar", sexpr.get_cdr().as_string());
  ASSERT_EQ(sexp::Value::TYPE_CONS, sexpr.get_type());
  ASSERT_EQ(sexp::Value::TYPE_SYMBOL, sexpr.get_car().get_type());
  ASSERT_EQ(sexp::Value::TYPE_SYMBOL, sexpr.get_cdr().get_type());

  ASSERT_EQ("(foo . bar)", sexp::Parser::from_string("(foo . bar)").str());
}

TEST(ParserTest, list_pair)
{
  sexp::Value sexpr = sexp::Parser::from_string("(1 2 3 4 5 . 6)");
  ASSERT_EQ("(1 2 3 4 5 . 6)", sexpr.str());
}

/* EOF */
