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

/* EOF */
