#include <gtest/gtest.h>

#include <sstream>

#include "sexp/value.hpp"

TEST(ValueTest, simple)
{
  using sexp::Value;
  using sexp::ValueImpl;

  auto sexpr_bool = Value::boolean(true);
  auto sexpr_int = Value::integer(5);
  auto sexpr_float = Value::real(45.0f);
  auto sexpr_cons = Value::cons(Value::integer(5), Value::nil());
  auto sexpr_cons2 = Value::cons(std::move(sexpr_int), Value::nil());

  ASSERT_EQ(5, sexpr_cons2.get_car().as_int());
  ASSERT_EQ(Value::nil(), sexpr_cons2.get_cdr());
}

/* EOF */
