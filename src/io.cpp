// SExp - A S-Expression Parser for C++
// Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//               2015 Ingo Ruhnke <grumbel@gmail.com>
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

#include "sexp/io.hpp"

namespace sexp {

std::ostream& operator<<(std::ostream& os, SExpr const& sexpr)
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

} // namespace sexp

/* EOF */
