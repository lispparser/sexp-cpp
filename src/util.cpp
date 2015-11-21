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

#include "sexp/util.hpp"

#include <sstream>
#include <stdexcept>

#include "sexp/io.hpp"

namespace sexp {

Value& car(Value& sx) { return sx.get_car(); }
Value& cdr(Value& sx) { return sx.get_cdr(); }
Value& caar(Value& sx) { return sx.get_car().get_car(); }
Value& cadr(Value& sx) { return sx.get_car().get_cdr(); }
Value& cdar(Value& sx) { return sx.get_cdr().get_car(); }
Value& cddr(Value& sx) { return sx.get_cdr().get_cdr(); }
Value& caaar(Value& sx) { return sx.get_car().get_car().get_car(); }
Value& caadr(Value& sx) { return sx.get_car().get_car().get_cdr(); }
Value& cadar(Value& sx) { return sx.get_car().get_cdr().get_car(); }
Value& caddr(Value& sx) { return sx.get_car().get_cdr().get_cdr(); }
Value& cdaar(Value& sx) { return sx.get_cdr().get_car().get_car(); }
Value& cdadr(Value& sx) { return sx.get_cdr().get_car().get_cdr(); }
Value& cddar(Value& sx) { return sx.get_cdr().get_cdr().get_car(); }
Value& cdddr(Value& sx) { return sx.get_cdr().get_cdr().get_cdr(); }
Value& caaaar(Value& sx) { return sx.get_car().get_car().get_car().get_car(); }
Value& caaadr(Value& sx) { return sx.get_car().get_car().get_car().get_cdr(); }
Value& caadar(Value& sx) { return sx.get_car().get_car().get_cdr().get_car(); }
Value& caaddr(Value& sx) { return sx.get_car().get_car().get_cdr().get_cdr(); }
Value& cadaar(Value& sx) { return sx.get_car().get_cdr().get_car().get_car(); }
Value& cadadr(Value& sx) { return sx.get_car().get_cdr().get_car().get_cdr(); }
Value& caddar(Value& sx) { return sx.get_car().get_cdr().get_cdr().get_car(); }
Value& cadddr(Value& sx) { return sx.get_car().get_cdr().get_cdr().get_cdr(); }
Value& cdaaar(Value& sx) { return sx.get_cdr().get_car().get_car().get_car(); }
Value& cdaadr(Value& sx) { return sx.get_cdr().get_car().get_car().get_cdr(); }
Value& cdadar(Value& sx) { return sx.get_cdr().get_car().get_cdr().get_car(); }
Value& cdaddr(Value& sx) { return sx.get_cdr().get_car().get_cdr().get_cdr(); }
Value& cddaar(Value& sx) { return sx.get_cdr().get_cdr().get_car().get_car(); }
Value& cddadr(Value& sx) { return sx.get_cdr().get_cdr().get_car().get_cdr(); }
Value& cdddar(Value& sx) { return sx.get_cdr().get_cdr().get_cdr().get_car(); }
Value& cddddr(Value& sx) { return sx.get_cdr().get_cdr().get_cdr().get_cdr(); }

Value const& car(Value const& sx) { return sx.get_car(); }
Value const& cdr(Value const& sx) { return sx.get_cdr(); }
Value const& caar(Value const& sx) { return sx.get_car().get_car(); }
Value const& cadr(Value const& sx) { return sx.get_car().get_cdr(); }
Value const& cdar(Value const& sx) { return sx.get_cdr().get_car(); }
Value const& cddr(Value const& sx) { return sx.get_cdr().get_cdr(); }
Value const& caaar(Value const& sx) { return sx.get_car().get_car().get_car(); }
Value const& caadr(Value const& sx) { return sx.get_car().get_car().get_cdr(); }
Value const& cadar(Value const& sx) { return sx.get_car().get_cdr().get_car(); }
Value const& caddr(Value const& sx) { return sx.get_car().get_cdr().get_cdr(); }
Value const& cdaar(Value const& sx) { return sx.get_cdr().get_car().get_car(); }
Value const& cdadr(Value const& sx) { return sx.get_cdr().get_car().get_cdr(); }
Value const& cddar(Value const& sx) { return sx.get_cdr().get_cdr().get_car(); }
Value const& cdddr(Value const& sx) { return sx.get_cdr().get_cdr().get_cdr(); }
Value const& caaaar(Value const& sx) { return sx.get_car().get_car().get_car().get_car(); }
Value const& caaadr(Value const& sx) { return sx.get_car().get_car().get_car().get_cdr(); }
Value const& caadar(Value const& sx) { return sx.get_car().get_car().get_cdr().get_car(); }
Value const& caaddr(Value const& sx) { return sx.get_car().get_car().get_cdr().get_cdr(); }
Value const& cadaar(Value const& sx) { return sx.get_car().get_cdr().get_car().get_car(); }
Value const& cadadr(Value const& sx) { return sx.get_car().get_cdr().get_car().get_cdr(); }
Value const& caddar(Value const& sx) { return sx.get_car().get_cdr().get_cdr().get_car(); }
Value const& cadddr(Value const& sx) { return sx.get_car().get_cdr().get_cdr().get_cdr(); }
Value const& cdaaar(Value const& sx) { return sx.get_cdr().get_car().get_car().get_car(); }
Value const& cdaadr(Value const& sx) { return sx.get_cdr().get_car().get_car().get_cdr(); }
Value const& cdadar(Value const& sx) { return sx.get_cdr().get_car().get_cdr().get_car(); }
Value const& cdaddr(Value const& sx) { return sx.get_cdr().get_car().get_cdr().get_cdr(); }
Value const& cddaar(Value const& sx) { return sx.get_cdr().get_cdr().get_car().get_car(); }
Value const& cddadr(Value const& sx) { return sx.get_cdr().get_cdr().get_car().get_cdr(); }
Value const& cdddar(Value const& sx) { return sx.get_cdr().get_cdr().get_cdr().get_car(); }
Value const& cddddr(Value const& sx) { return sx.get_cdr().get_cdr().get_cdr().get_cdr(); }

bool
is_list(Value const& sx)
{
  if (sx.is_nil())
  {
    return true;
  }
  else if (sx.is_cons())
  {
    return is_list(sx.get_cdr());
  }
  else
  {
    return false;
  }
}

int
list_length(Value const& sx)
{
  if (sx.is_nil())
  {
    return 0;
  }
  else if (sx.is_cons())
  {
    return 1 + list_length(sx.get_cdr());
  }
  else
  {
    // silently ignoring malformed list content
    return 0;
  }
}

Value const&
list_ref(Value const& sx, int index)
{
  if (index == 0)
  {
    return sx.get_car();
  }
  else
  {
    return list_ref(sx.get_cdr(), index - 1);
  }
}

Value const&
assoc_ref(Value const& sx, std::string const& key)
{
  if (sx.is_nil())
  {
    return Value::nil_ref();
  }
  else if (sx.is_cons())
  {
    Value const& pair = sx.get_car();
    if (pair.is_cons() &&
        pair.get_car().is_symbol() &&
        pair.get_car().as_string() == key)
    {
      return pair.get_cdr();
    }
    else
    {
      return assoc_ref(sx.get_cdr(), key);
    }
  }
  else
  {
    std::ostringstream msg;
    msg << "malformed input to sexp::assoc_ref(): sx:\"" << sx << "\" key:\"" << key << "\"";
    throw std::runtime_error(msg.str());
  }
}

} // namespace sexp

/* EOF */
