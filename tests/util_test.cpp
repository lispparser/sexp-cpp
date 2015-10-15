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

#include "sexp/parser.hpp"
#include "sexp/util.hpp"
#include "sexp/value.hpp"

TEST(UtilTest, is_list)
{
  EXPECT_TRUE(sexp::is_list(sexp::Parser::from_string("()")));
  EXPECT_TRUE(sexp::is_list(sexp::Parser::from_string("(1)")));
  EXPECT_TRUE(sexp::is_list(sexp::Parser::from_string("(1 2)")));
  EXPECT_TRUE(sexp::is_list(sexp::Parser::from_string("(1 2 3)")));
  EXPECT_TRUE(sexp::is_list(sexp::Parser::from_string("((1) (2 3) (3 4 5))")));
  EXPECT_TRUE(sexp::is_list(sexp::Parser::from_string("(1 . (2 . (3 . (4 . (5 . ())))))")));

  EXPECT_FALSE(sexp::is_list(sexp::Parser::from_string("(1 . 5)")));
  EXPECT_FALSE(sexp::is_list(sexp::Parser::from_string("(1 2 3 4 . 5)")));
  EXPECT_FALSE(sexp::is_list(sexp::Parser::from_string("(1 . (2 . (3 . 5)))")));
}

TEST(UtilTest, list_iterator)
{
  sexp::Value lst = sexp::Parser::from_string("(1 2 3 4 5 6 7 8 9)");
  std::vector<int> result;
  for(sexp::ListIterator it(lst); it != sexp::ListIterator(); ++it)
  {
    result.push_back(it->as_int());
  }
  std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9};
  ASSERT_EQ(expected, result);
}

TEST(UtilTest, list_iterator_invalid)
{
  // the list isn't terminated with (), but with 10, it should get silently ignored
  sexp::Value lst = sexp::Parser::from_string("(1 2 3 4 5 6 7 8 9 . 10)");
  std::vector<int> result;
  for(sexp::ListIterator it(lst); it != sexp::ListIterator(); ++it)
  {
    result.push_back(it->as_int());
  }
  std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9};
  ASSERT_EQ(expected, result);
}

/* EOF */
