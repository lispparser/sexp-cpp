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

#ifndef HEADER_SEXP_UTIL_HPP
#define HEADER_SEXP_UTIL_HPP

namespace sexp {

class Value;

bool is_list(Value const& sx);

class ListIterator
{
private:
  Value const* cur;

public:
  ListIterator() :
    cur(nullptr)
  {}

  ListIterator(Value const& sx) :
    cur(&sx)
  {}

  bool operator==(ListIterator const& rhs) const { return cur == rhs.cur; }
  bool operator!=(ListIterator const& rhs) const { return cur != rhs.cur; }

  Value const& operator*() const { return cur->get_car(); }
  Value const* operator->() const { return &cur->get_car(); }

  ListIterator& operator++()
  {
    if (cur)
    {
      cur = &cur->get_cdr();
      if (!cur->is_cons())
      {
        cur = nullptr;
      }
    }
    return *this;
  }

  ListIterator operator++(int)
  {
    ListIterator tmp = *this;
    operator++();
    return tmp;
  }
};

} // namespace sexp

#endif

/* EOF */
