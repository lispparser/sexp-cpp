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
  struct Cons;

  Value::Type m_type;
  union
  {
    bool m_bool;
    int m_int;
    float m_float;

    std::string* m_string;
    Cons* m_cons;
  };

public:
  /** Returns a reference to a nil value for use in functions that
      return a reference and need to return a nil value */
  static Value const& nil_ref() { static Value const s_nil; return s_nil; }
  static Value nil() { return Value(); }
  static Value boolean(bool v) { return Value(v); }
  static Value integer(int v) { return Value(v); }
  static Value real(float v) { return Value(v); }
  static Value string(std::string const& v) { return Value(TYPE_STRING, v); }
  static Value symbol(std::string const& v) { return Value(TYPE_SYMBOL, v); }
  static Value cons(Value&& car, Value&& cdr) { return Value(std::move(car), std::move(cdr)); }
  static Value cons() { return Value(Value::nil(), Value::nil()); }

private:
  inline Value(bool value) : m_type(TYPE_BOOLEAN), m_bool(value) {}
  inline Value(int value) : m_type(TYPE_INTEGER), m_int(value) {}
  inline Value(float value) : m_type(TYPE_REAL), m_float(value) {}
  inline Value(Type type, std::string const& value) :
    m_type(type),
    m_string(new std::string(value))
  {}
  inline Value(Value&& car, Value&& cdr);

  void destroy();

public:
  explicit Value(Value const& other);

  inline Value(Value&& other) :
    m_type(other.m_type)
  {
    switch(m_type)
    {
      case TYPE_NIL:
        break;

      case TYPE_BOOLEAN:
        m_bool = other.m_bool;
        break;

      case TYPE_INTEGER:
        m_int = other.m_int;
        break;

      case TYPE_REAL:
        m_float = other.m_float;
        break;

      case TYPE_STRING:
      case TYPE_SYMBOL:
        m_string = other.m_string;
        break;

      case TYPE_CONS:
        m_cons = other.m_cons;
        break;
    }

    other.m_type = TYPE_NIL;
  }

  inline Value() :
    m_type(TYPE_NIL)
  {}

  inline ~Value()
  {
    destroy();
  }

  inline Value& operator=(Value&& other)
  {
    destroy();

    m_type = other.m_type;
    other.m_type = TYPE_NIL;

    switch(m_type)
    {
      case TYPE_NIL:
        break;

      case TYPE_BOOLEAN:
        m_bool = other.m_bool;
        break;

      case TYPE_INTEGER:
        m_int = other.m_int;
        break;

      case TYPE_REAL:
        m_float = other.m_float;
        break;

      case TYPE_STRING:
      case TYPE_SYMBOL:
        m_string = other.m_string;
        break;

      case TYPE_CONS:
        m_cons = other.m_cons;
        break;
    }

    return *this;
  }

  inline Type get_type() const { return m_type; }

  inline explicit operator bool() const { return m_type != TYPE_NIL; }

  inline bool is_nil() const { return m_type == TYPE_NIL; }
  inline bool is_boolean() const { return m_type == TYPE_BOOLEAN; }
  inline bool is_integer() const { return m_type == TYPE_INTEGER; }
  inline bool is_real() const { return (m_type == TYPE_REAL || m_type == TYPE_INTEGER); }
  inline bool is_string() const { return m_type == TYPE_STRING; }
  inline bool is_symbol() const { return m_type == TYPE_SYMBOL; }
  inline bool is_cons() const { return m_type == TYPE_CONS; }

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

struct Value::Cons
{
  Value car;
  Value cdr;
};

inline
Value::Value(Value&& car, Value&& cdr) :
  m_type(TYPE_CONS),
  m_cons(new Cons{std::move(car), std::move(cdr)})
{}

inline void
Value::destroy()
{
  switch(m_type)
  {
    case Value::TYPE_STRING:
    case Value::TYPE_SYMBOL:
      delete m_string;
      break;

    case Value::TYPE_CONS:
      delete m_cons;
      break;

    default:
      // atoms don't need deletion
      break;
  }
}

inline
Value::Value(Value const& other) :
  m_type(other.m_type)
{
  switch(m_type)
  {
    case TYPE_NIL:
      break;

    case TYPE_BOOLEAN:
      m_bool = other.m_bool;
      break;

    case TYPE_INTEGER:
      m_int = other.m_int;
      break;

    case TYPE_REAL:
      m_float = other.m_float;
      break;

    case TYPE_STRING:
    case TYPE_SYMBOL:
      m_string = new std::string(*other.m_string);
      break;

    case TYPE_CONS:
      m_cons = new Cons(*other.m_cons);
      break;
  }
}

inline bool
Value::operator==(Value const& rhs) const
{
  if (m_type == rhs.m_type)
  {
    switch(m_type)
    {
      case TYPE_NIL:
        return true;

      case Value::TYPE_BOOLEAN:
        return m_bool == rhs.m_bool;

      case Value::TYPE_INTEGER:
        return m_int == rhs.m_int;

      case Value::TYPE_REAL:
        return m_float == rhs.m_float;

      case Value::TYPE_STRING:
      case Value::TYPE_SYMBOL:
        return *m_string == *rhs.m_string;

      case Value::TYPE_CONS:
        return (m_cons->car == rhs.m_cons->car &&
                m_cons->cdr == rhs.m_cons->cdr);
    }
    assert(false && "should never be reached");
    return false;
  }
  else
  {
    return false;
  }
}

inline Value const&
Value::get_car() const
{
  assert(m_type == TYPE_CONS);
  return m_cons->car;
}

inline Value const&
Value::get_cdr() const
{
  assert(m_type == TYPE_CONS);
  return m_cons->cdr;
}

inline Value&
Value::get_car()
{
  assert(m_type == TYPE_CONS);
  return m_cons->car;
}

inline Value&
Value::get_cdr()
{
  assert(m_type == TYPE_CONS);
  return m_cons->cdr;
}

inline void
Value::set_car(Value&& sexpr)
{
  assert(m_type == TYPE_CONS);
  m_cons->car = std::move(sexpr);
}

inline void
Value::set_cdr(Value&& sexpr)
{
  assert(m_type == TYPE_CONS);
  m_cons->cdr = std::move(sexpr);
}

inline bool
Value::as_bool() const
{
  assert(m_type == TYPE_BOOLEAN);
  return m_bool;
}

inline int
Value::as_int() const
{
  assert(m_type == TYPE_INTEGER);
  return m_int;
}

inline float
Value::as_float() const
{
  if (m_type == TYPE_REAL)
  {
    return m_float;
  }
  else if (m_type == TYPE_INTEGER)
  {
    return static_cast<float>(m_int);
  }
  else
  {
    assert(false && "type missmatch");
    return 0.0f;
  }
}

inline std::string const&
Value::as_string() const
{
  assert(m_type == TYPE_SYMBOL ||
         m_type == TYPE_STRING);
  return *m_string;
}

} // namespace sexp

#endif

/* EOF */
