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

#include "sexp/value.hpp"

TEST(ValueTest, simple)
{
  using sexp::Value;
  using sexp::ValueImpl;

  auto sx_bool = Value::boolean(true);
  auto sx_int = Value::integer(5);
  auto sx_float = Value::real(45.0f);
  auto sx_cons = Value::cons(Value::integer(5), Value::nil());
  auto sx_cons2 = Value::cons(std::move(sx_int), Value::nil());

  ASSERT_EQ(5, sx_cons2.get_car().as_int());
  ASSERT_EQ(Value::nil(), sx_cons2.get_cdr());
}

/* EOF */
