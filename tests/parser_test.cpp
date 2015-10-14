#include <gtest/gtest.h>

#include <sstream>

#include "lisp/io.hpp"
#include "lisp/sexpr.hpp"
#include "lisp/parser.hpp"

TEST(ParserTest, simple)
{
  std::istringstream in("(1 2.5 foo \"TEXT\" bar bar)");
  lisp::SExpr cons = lisp::Parser::from_stream(in);
}

TEST(ParserTest, parse_integer)
{
  auto sexpr = lisp::Parser::from_string("12345");
  ASSERT_EQ(12345, sexpr.as_int());
}

TEST(ParserTest, parse_real)
{
  auto sexpr = lisp::Parser::from_string("0.125");
  ASSERT_EQ(lisp::SExpr::TYPE_REAL, sexpr.get_type());
  ASSERT_EQ(0.125f, sexpr.as_float());

  sexpr = lisp::Parser::from_string(".125");
  ASSERT_EQ(lisp::SExpr::TYPE_REAL, sexpr.get_type());
  ASSERT_EQ(0.125f, sexpr.as_float());
}

TEST(ParserTest, parse_string)
{
  auto sexpr = lisp::Parser::from_string("\"Hello\\nWorld\"");
  ASSERT_EQ(lisp::SExpr::TYPE_STRING, sexpr.get_type());
  ASSERT_EQ("Hello\nWorld", sexpr.as_string());
}

TEST(ParserTest, parse_symbol)
{
  auto sexpr = lisp::Parser::from_string("HelloWorld");
  ASSERT_EQ(lisp::SExpr::TYPE_SYMBOL, sexpr.get_type());
  ASSERT_EQ("HelloWorld", sexpr.as_string());
}

// FIXME: Compare data structure or use simple strings?!
// "(foo . bar)" as string is ambigous in the current parser as . can be handled as symbol, not pair
TEST(ParserTest, simple_pair)
{
  lisp::SExpr sexpr = lisp::Parser::from_string("(foo . bar)");
  std::ostringstream os;
  os << sexpr;
  ASSERT_EQ("(foo . bar)", os.str());
  ASSERT_EQ("foo", sexpr.get_car().as_string());
  ASSERT_EQ("bar", sexpr.get_cdr().as_string());
  ASSERT_EQ(lisp::SExpr::TYPE_CONS, sexpr.get_type());
  ASSERT_EQ(lisp::SExpr::TYPE_SYMBOL, sexpr.get_car().get_type());
  ASSERT_EQ(lisp::SExpr::TYPE_SYMBOL, sexpr.get_cdr().get_type());
}

TEST(ParserTest, list_pair)
{
  lisp::SExpr sexpr = lisp::Parser::from_string("(1 2 3 4 5 . 6)");
  std::ostringstream os;
  os << sexpr;
  ASSERT_EQ("(1 2 3 4 5 . 6)", os.str());
}

/* EOF */
