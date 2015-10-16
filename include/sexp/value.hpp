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

class ValueImpl;

class Value
{
public:
  enum Type
  {
    TYPE_NIL,
    TYPE_BOOLEAN,
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_STRING,
    TYPE_SYMBOL,
    TYPE_CONS
  };

private:
  std::unique_ptr<ValueImpl> m_impl;

public:
  /** Returns a reference to a nil value for use in functions that
      return a reference and need to return a nil value */
  static Value const& nil_ref() { static Value const s_nil; return s_nil; }
  static Value nil() { return Value(); }
  static Value boolean(bool v) { return Value(std::make_unique<ValueImpl>(v)); }
  static Value integer(int v) { return Value(std::make_unique<ValueImpl>(v)); }
  static Value real(float v) { return Value(std::make_unique<ValueImpl>(v)); }
  static Value string(std::string const& v) { return Value(std::make_unique<ValueImpl>(TYPE_STRING, v)); }
  static Value symbol(std::string const& v) { return Value(std::make_unique<ValueImpl>(TYPE_SYMBOL, v)); }
  static Value cons(Value&& car, Value&& cdr) { return Value(std::make_unique<ValueImpl>(std::move(car), std::move(cdr))); }
  static Value cons() { return Value(std::make_unique<ValueImpl>(Value::nil(), Value::nil())); }

public:
  explicit Value(Value const& other) :
    m_impl(other.is_nil() ? nullptr : std::make_unique<ValueImpl>(*other.m_impl))
  {}

  Value(Value&& other) :
    m_impl(std::move(other.m_impl))
  {}

  Value() :
    m_impl()
  {}

  Value(std::unique_ptr<ValueImpl> impl) :
    m_impl(std::move(impl))
  {}

  ~Value()
  {}

  Value& operator=(Value&& other)
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

  Value const& get_car() const;
  Value const& get_cdr() const;

  Value& get_car();
  Value& get_cdr();

  void set_car(Value&& sexpr);
  void set_cdr(Value&& sexpr);

  bool as_bool() const;
  int as_int() const;
  float as_float() const;
  std::string const& as_symbol() const;
  std::string const& as_string() const;

  bool operator==(Value const& other) const;

  std::string str() const;
};

class ValueImpl
{
public:
  Value::Type m_type;
  union
  {
    struct
    {
      Value car;
      Value cdr;
    } m_cons;

    std::string* m_string;
    bool m_bool;
    int m_int;
    float m_float;
  };

  ValueImpl(ValueImpl const& other) :
    m_type(other.m_type)
  {
    switch(m_type)
    {
      case Value::TYPE_NIL:
        assert(false && "ValueImpl can't be TYPE_NIL");
        break;

      case Value::TYPE_BOOLEAN:
        m_bool = other.m_bool;
        break;

      case Value::TYPE_INTEGER:
        m_int = other.m_int;
        break;

      case Value::TYPE_REAL:
        m_float = other.m_float;
        break;

      case Value::TYPE_CONS:
        new (&m_cons.car) Value(other.m_cons.car);
        new (&m_cons.cdr) Value(other.m_cons.cdr);
        break;

      case Value::TYPE_SYMBOL:
      case Value::TYPE_STRING:
        m_string = new std::string(*other.m_string);
        break;
    }
  }

  ValueImpl(Value&& car, Value&& cdr) :
    m_type(Value::TYPE_CONS),
    m_cons{std::move(car), std::move(cdr)}
  {}

  ValueImpl(Value::Type type, std::string const& text) :
    m_type(type),
    m_string(new std::string(text))
  {}

  ValueImpl(bool v) :
    m_type(Value::TYPE_BOOLEAN),
    m_bool(v)
  {}

  ValueImpl(int v) :
    m_type(Value::TYPE_INTEGER),
    m_int(v)
  {}

  ValueImpl(float v) :
    m_type(Value::TYPE_REAL),
    m_float(v)
  {}

  ~ValueImpl()
  {
    switch(m_type)
    {
      case Value::TYPE_STRING:
      case Value::TYPE_SYMBOL:
        delete m_string;
        break;

      case Value::TYPE_CONS:
        m_cons.car.~Value();
        m_cons.cdr.~Value();
        break;

      default:
        // atoms don't need deletion
        break;
    }
  }

  bool operator==(ValueImpl const& rhs) const
  {
    if (m_type != rhs.m_type)
    {
      return false;
    }
    else
    {
      switch(m_type)
      {
        case Value::TYPE_BOOLEAN:
          return m_bool == rhs.m_bool;

        case Value::TYPE_INTEGER:
          return m_int == rhs.m_int;

        case Value::TYPE_REAL:
          return m_float == rhs.m_float;

        case Value::TYPE_STRING:
        case Value::TYPE_SYMBOL:

        case Value::TYPE_CONS:
          return (m_cons.car == m_cons.car &&
                  m_cons.cdr == m_cons.cdr);

        default:
          assert(false && "should never be reached");
      }
    }
  }
};

inline bool
Value::operator==(Value const& other) const
{
  if (m_impl == other.m_impl)
  {
    return true;
  }
  else if (m_impl)
  {
    return *m_impl == *other.m_impl;
  }
  else
  {
    return false;
  }
}

inline Value::Type
Value::get_type() const
{
  return (m_impl ? m_impl->m_type : Value::TYPE_NIL);
}

inline bool
Value::is_nil() const
{
  return m_impl == nullptr;
}

inline bool
Value::is_cons() const
{
  return m_impl && m_impl->m_type == Value::TYPE_CONS;
}

inline bool
Value::is_symbol() const
{
  return m_impl && m_impl->m_type == Value::TYPE_SYMBOL;
}

inline bool
Value::is_string() const
{
  return m_impl && m_impl->m_type == Value::TYPE_STRING;
}

inline bool
Value::is_integer() const
{
  return m_impl && m_impl->m_type == Value::TYPE_INTEGER;
}

inline bool
Value::is_real() const
{
  return m_impl && (m_impl->m_type == Value::TYPE_REAL ||
                    m_impl->m_type == Value::TYPE_INTEGER);
}

inline Value const&
Value::get_car() const
{
  assert(m_impl->m_type == Value::TYPE_CONS);
  return m_impl->m_cons.car;
}

inline Value const&
Value::get_cdr() const
{
  assert(m_impl->m_type == Value::TYPE_CONS);
  return m_impl->m_cons.cdr;
}

inline Value&
Value::get_car()
{
  assert(m_impl->m_type == Value::TYPE_CONS);
  return m_impl->m_cons.car;
}

inline Value&
Value::get_cdr()
{
  assert(m_impl->m_type == Value::TYPE_CONS);
  return m_impl->m_cons.cdr;
}

inline void
Value::set_car(Value&& sexpr)
{
  assert(m_impl->m_type == Value::TYPE_CONS);
  m_impl->m_cons.car = std::move(sexpr);
}

inline void
Value::set_cdr(Value&& sexpr)
{
  assert(m_impl->m_type == Value::TYPE_CONS);
  m_impl->m_cons.cdr = std::move(sexpr);
}

inline bool
Value::as_bool() const
{
  assert(m_impl->m_type == Value::TYPE_BOOLEAN);
  return m_impl->m_bool;
}

inline int
Value::as_int() const
{
  assert(m_impl->m_type == Value::TYPE_INTEGER);
  return m_impl->m_int;
}

inline float
Value::as_float() const
{
  if (m_impl->m_type == Value::TYPE_REAL)
  {
    return m_impl->m_float;
  }
  else if (m_impl->m_type == Value::TYPE_INTEGER)
  {
    return static_cast<float>(m_impl->m_int);
  }
  else
  {
    assert(false && "type missmatch");
  }
}

inline std::string const&
Value::as_string() const
{
  assert(m_impl->m_type == Value::TYPE_SYMBOL ||
         m_impl->m_type == Value::TYPE_STRING);
  return *m_impl->m_string;
}

} // namespace sexp

#endif

/* EOF */
