#ifndef HEADER_IO_HPP
#define HEADER_IO_HPP

#include <ostream>

#include "lisp/sexpr.hpp"

namespace lisp {

inline std::ostream&
operator<<(std::ostream& os, SExpr const& sexpr)
{
  switch(sexpr.get_type())
  {
    case SExpr::TYPE_NIL:
      os << "()";
      break;

    case SExpr::TYPE_CONS:
      {
        os << '(';
        SExpr const* cur = &sexpr;
        do
        {
          if (cur->get_type() != SExpr::TYPE_CONS)
          {
            os << ". " << *cur;
            break;
          }
          else
          {
            os << cur->get_car();
            cur = &cur->get_cdr();
            if (*cur)
            {
              os << ' ';
            }
          }
        }
        while(*cur);
        os << ')';
      }
      break;

    case SExpr::TYPE_STRING:
      os << '"' << sexpr.as_string() << '"';
      break;

    case SExpr::TYPE_INTEGER:
      os << sexpr.as_int();
      break;

    case SExpr::TYPE_REAL:
      os << sexpr.as_float();
      break;

    case SExpr::TYPE_SYMBOL:
      os << sexpr.as_string();
      break;

    case SExpr::TYPE_BOOLEAN:
      os << (sexpr.as_bool() ? "#t" : "#f");
      break;
  }

  return os;
}

} // namespace lisp

#endif

/* EOF */
