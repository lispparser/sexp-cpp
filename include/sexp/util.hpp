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

#include <sexp/value.hpp>

namespace sexp {

class Value;

int list_length(Value const& sx);
Value const& list_ref(Value const& sx, int index);
bool is_list(Value const& sx);
Value const& assoc_ref(Value const& sx, std::string const& key);

class ListIterator
{
private:
  Value const* cur;

public:
  ListIterator() :
    cur(nullptr)
  {}

  ListIterator(Value const& sx) :
    cur(!sx.is_cons() ? nullptr : &sx)
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
        // if the list is malformed we stop at the last valid element
        // and silently ignore the rest
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

class ListAdapter
{
private:
  Value const& m_sx;

public:
  ListAdapter(Value const& sx) :
    m_sx(sx)
  {}

  ListIterator begin() const { return ListIterator(m_sx); }
  ListIterator end() const { return ListIterator(); }
};

} // namespace sexp

#endif

/* EOF */
