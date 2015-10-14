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

#ifndef HEADER_SEXP_VALUE_HPP
#define HEADER_SEXP_VALUE_HPP

#include <assert.h>
#include <memory>

namespace sexp {

class SExprImpl;

class SExpr
{
public:
  enum Type
  {
    TYPE_NIL,
    TYPE_CONS,
    TYPE_SYMBOL,
    TYPE_INTEGER,
    TYPE_STRING,
    TYPE_REAL,
    TYPE_BOOLEAN
  };

private:
  std::unique_ptr<SExprImpl> m_impl;

public:
  static SExpr nil() { return SExpr(); }
  static SExpr boolean(bool v) { return SExpr(std::make_unique<SExprImpl>(v)); }
  static SExpr integer(int v) { return SExpr(std::make_unique<SExprImpl>(v)); }
  static SExpr real(float v) { return SExpr(std::make_unique<SExprImpl>(v)); }
  static SExpr string(std::string const& v) { return SExpr(std::make_unique<SExprImpl>(TYPE_STRING, v)); }
  static SExpr symbol(std::string const& v) { return SExpr(std::make_unique<SExprImpl>(TYPE_SYMBOL, v)); }
  static SExpr cons(SExpr&& car, SExpr&& cdr) { return SExpr(std::make_unique<SExprImpl>(std::move(car), std::move(cdr))); }
  static SExpr cons() { return SExpr(std::make_unique<SExprImpl>(SExpr::nil(), SExpr::nil())); }

public:
  SExpr(SExpr&& other) :
    m_impl(std::move(other.m_impl))
  {}

  SExpr() :
    m_impl()
  {}

  SExpr(std::unique_ptr<SExprImpl> impl) :
    m_impl(std::move(impl))
  {}

  ~SExpr()
  {}

  SExpr& operator=(SExpr&& other)
  {
    m_impl = std::move(other.m_impl);
    return *this;
  }

  Type get_type() const;

  explicit operator bool() const
  {
    return static_cast<bool>(m_impl);
  }

  bool is_nil() const;
  bool is_cons() const;
  bool is_symbol() const;
  bool is_string() const;
  bool is_integer() const;
  bool is_real() const;

  SExpr const& get_car() const;
  SExpr const& get_cdr() const;

  SExpr& get_car();
  SExpr& get_cdr();

  void set_car(SExpr&& sexpr);
  void set_cdr(SExpr&& sexpr);

  bool as_bool() const;
  int as_int() const;
  float as_float() const;
  std::string const& as_symbol() const;
  std::string const& as_string() const;

  bool operator==(SExpr const& other) const
  {
    if (m_impl == other.m_impl)
    {
      return true;
    }
    else
    {
      // FIXME: Implement some tree traversal
      return false;
    }
  }

  std::string str() const;
};

class SExprImpl
{
public:
  SExpr::Type m_type;
  union
  {
    struct
    {
      SExpr car;
      SExpr cdr;
    } m_cons;

    std::string* m_string;
    bool m_bool;
    int m_int;
    float m_float;
  };

  SExprImpl(SExpr&& car, SExpr&& cdr) :
    m_type(SExpr::TYPE_CONS),
    m_cons{std::move(car), std::move(cdr)}
  {}

  SExprImpl(SExpr::Type type, std::string const& text) :
    m_type(type),
    m_string(new std::string(text))
  {}

  SExprImpl(bool v) :
    m_type(SExpr::TYPE_BOOLEAN),
    m_bool(v)
  {}

  SExprImpl(int v) :
    m_type(SExpr::TYPE_INTEGER),
    m_int(v)
  {}

  SExprImpl(float v) :
    m_type(SExpr::TYPE_REAL),
    m_float(v)
  {}

  ~SExprImpl()
  {
    switch(m_type)
    {
      case SExpr::TYPE_STRING:
      case SExpr::TYPE_SYMBOL:
        delete m_string;
        break;

      case SExpr::TYPE_CONS:
        m_cons.car.~SExpr();
        m_cons.cdr.~SExpr();
        break;

      default:
        // atoms don't need deletion
        break;
    }
  }
};

inline SExpr::Type
SExpr::get_type() const
{
  return (m_impl ? m_impl->m_type : SExpr::TYPE_NIL);
}

inline bool
SExpr::is_nil() const
{
  return m_impl == nullptr;
}

inline bool
SExpr::is_cons() const
{
  return m_impl && m_impl->m_type == SExpr::TYPE_CONS;
}

inline bool
SExpr::is_symbol() const
{
  return m_impl && m_impl->m_type == SExpr::TYPE_SYMBOL;
}

inline bool
SExpr::is_string() const
{
  return m_impl && m_impl->m_type == SExpr::TYPE_STRING;
}

inline bool
SExpr::is_integer() const
{
  return m_impl && m_impl->m_type == SExpr::TYPE_INTEGER;
}

inline bool
SExpr::is_real() const
{
  return m_impl && (m_impl->m_type == SExpr::TYPE_REAL ||
                    m_impl->m_type == SExpr::TYPE_INTEGER);
}

inline SExpr const&
SExpr::get_car() const
{
  assert(m_impl->m_type == SExpr::TYPE_CONS);
  return m_impl->m_cons.car;
}

inline SExpr const&
SExpr::get_cdr() const
{
  assert(m_impl->m_type == SExpr::TYPE_CONS);
  return m_impl->m_cons.cdr;
}

inline SExpr&
SExpr::get_car()
{
  assert(m_impl->m_type == SExpr::TYPE_CONS);
  return m_impl->m_cons.car;
}

inline SExpr&
SExpr::get_cdr()
{
  assert(m_impl->m_type == SExpr::TYPE_CONS);
  return m_impl->m_cons.cdr;
}

inline void
SExpr::set_car(SExpr&& sexpr)
{
  assert(m_impl->m_type == SExpr::TYPE_CONS);
  m_impl->m_cons.car = std::move(sexpr);
}

inline void
SExpr::set_cdr(SExpr&& sexpr)
{
  assert(m_impl->m_type == SExpr::TYPE_CONS);
  m_impl->m_cons.cdr = std::move(sexpr);
}

inline bool
SExpr::as_bool() const
{
  assert(m_impl->m_type == SExpr::TYPE_BOOLEAN);
  return m_impl->m_bool;
}

inline int
SExpr::as_int() const
{
  assert(m_impl->m_type == SExpr::TYPE_INTEGER);
  return m_impl->m_int;
}

inline float
SExpr::as_float() const
{
  if (m_impl->m_type == SExpr::TYPE_REAL)
  {
    return m_impl->m_float;
  }
  else if (m_impl->m_type == SExpr::TYPE_INTEGER)
  {
    return static_cast<float>(m_impl->m_int);
  }
  else
  {
    assert(false && "type missmatch");
  }
}

inline std::string const&
SExpr::as_string() const
{
  assert(m_impl->m_type == SExpr::TYPE_SYMBOL ||
         m_impl->m_type == SExpr::TYPE_STRING);
  return *m_impl->m_string;
}

} // namespace sexp

#endif

/* EOF */
