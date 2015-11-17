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

#include <sstream>

namespace sexp {

void escape_string(std::ostream& os, std::string const& text)
{
  os << '"';
  for(size_t i = 0; i < text.size(); ++i)
  {
    if (text[i] == '"')
    {
      os << "\\\"";
    }
    else if (text[i] == '\\')
    {
      os << "\\\\";
    }
    else
    {
      os << text[i];
    }
  }
  os << '"';
}

std::ostream& operator<<(std::ostream& os, Value const& sx)
{
  switch(sx.get_type())
  {
    case Value::TYPE_NIL:
      os << "()";
      break;

    case Value::TYPE_CONS:
      {
        os << '(';
        Value const* cur = &sx;
        do
        {
          if (cur->get_type() != Value::TYPE_CONS)
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

    case Value::TYPE_STRING:
      escape_string(os, sx.as_string());
      break;

    case Value::TYPE_INTEGER:
      {
#ifdef SEXP_USE_LOCALE
        auto loc = os.getloc();
        os.imbue(std::locale::classic());
        os << sx.as_int();
        os.imbue(loc);
#else
        os << sx.as_int();
#endif
      }
      break;

    case Value::TYPE_REAL:
      {
#ifdef SEXP_USE_LOCALE
        auto loc = os.getloc();
        os.imbue(std::locale::classic());
        os << sx.as_float();
        os.imbue(loc);
#else
        os << sx.as_float();
#endif
      }
      break;

    case Value::TYPE_SYMBOL:
      os << sx.as_string();
      break;

    case Value::TYPE_BOOLEAN:
      os << (sx.as_bool() ? "#t" : "#f");
      break;

    case Value::TYPE_ARRAY:
      {
        os << "#(";
        auto const& arr = sx.as_array();
        for(size_t i = 0; i != arr.size(); ++i)
        {
          if (i != 0) os << ' ';
          os << arr[i];
        }
        os << ")";
      }
      break;
  }

  return os;
}

} // namespace sexp

/* EOF */
