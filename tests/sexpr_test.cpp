#include <gtest/gtest.h>

#include <sstream>

#include "lisp/sexpr.hpp"

TEST(SExprTest, simple)
{
  using lisp::SExpr;
  using lisp::SExprImpl;

  auto sexpr_bool = SExpr::boolean(true);
  auto sexpr_int = SExpr::integer(5);
  auto sexpr_float = SExpr::real(45.0f);
  auto sexpr_cons = SExpr::cons(SExpr::integer(5), SExpr::nil());
  auto sexpr_cons2 = SExpr::cons(std::move(sexpr_int), SExpr::nil());

  ASSERT_EQ(5, sexpr_cons2.get_car().as_int());
  ASSERT_EQ(SExpr::nil(), sexpr_cons2.get_cdr());
}

/* EOF */
