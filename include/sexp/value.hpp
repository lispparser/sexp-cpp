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
#include <string.h>
#include <string>
#include <vector>
#include <sexp/error.hpp>
#include <stdint.h>

namespace sexp {

class Value
{
public:
  enum Type : unsigned char
  {
    TYPE_NIL,
    TYPE_BOOLEAN,
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_STRING,
    TYPE_SYMBOL,
    TYPE_CONS,
    TYPE_ARRAY
  };

private:
  struct Cons;

#if INTPTR_MAX == INT32_MAX
  unsigned m_line : 24;
#else
  int m_line;
#endif

  Value::Type m_type;

  union
  {
    bool m_bool;
    int m_int;
    float m_float;

    std::string* m_string;
    Cons* m_cons;
    std::vector<Value>* m_array;
  };

  struct BooleanDummy {};
  struct IntegerDummy {};
  struct RealDummy {};
  struct StringDummy {};
  struct SymbolDummy {};
  struct ConsDummy {};
  struct ArrayDummy {};

public:
  /** Returns a reference to a nil value for use in functions that
      return a reference and need to return a nil value */
  static Value const& nil_ref() { static Value const s_nil; return s_nil; }
  static Value nil() { return Value(); }
  static Value boolean(bool v) { return Value(BooleanDummy(), v); }
  static Value integer(int v) { return Value(IntegerDummy(), v); }
  static Value real(float v) { return Value(RealDummy(), v); }
  static Value string(std::string const& v) { return Value(StringDummy(), v); }
  static Value symbol(std::string const& v) { return Value(SymbolDummy(), v); }
  static Value cons(Value&& car, Value&& cdr) { return Value(ConsDummy(), std::move(car), std::move(cdr)); }
  static Value cons() { return Value(ConsDummy(), Value::nil(), Value::nil()); }

  static Value array(std::vector<Value> arr) { return Value(ArrayDummy(), std::move(arr)); }
  template<typename... Args>
  static Value array(Args&&... args) { return Value(ArrayDummy(), std::move(args)...); }

  static Value list()
  {
    return Value::nil();
  }

  template<typename... Args>
  static Value list(Value&& head, Args&&... rest)
  {
    return Value::cons(std::move(head), list(std::move(rest)...));
  }

  int get_line() const { return static_cast<int>(m_line); }
  void set_line(int line)
  {
#if INTPTR_MAX == INT32_MAX
    m_line = static_cast<unsigned int>(line) & 0xffffff;
#else
    m_line = line;
#endif
  }

private:
  inline explicit Value(BooleanDummy, bool value) : m_line(0), m_type(TYPE_BOOLEAN), m_bool(value) {}
  inline explicit Value(IntegerDummy, int value) : m_line(0), m_type(TYPE_INTEGER), m_int(value) {}
  inline explicit Value(RealDummy, float value) : m_line(0), m_type(TYPE_REAL), m_float(value) {}
  inline Value(StringDummy, std::string const& value) :
    m_line(0),
    m_type(TYPE_STRING),
    m_string(new std::string(value))
  {}
  inline Value(SymbolDummy, std::string const& value) :
    m_line(0),
    m_type(TYPE_SYMBOL),
    m_string(new std::string(value))
  {}
  inline Value(ConsDummy, Value&& car, Value&& cdr);
  inline Value(ArrayDummy, std::vector<Value> arr) :
    m_line(0),
    m_type(TYPE_ARRAY),
    m_array(new std::vector<Value>(std::move(arr)))
  {}
  template<typename... Args>
  inline Value(ArrayDummy, Args&&... args) :
    m_line(0),
    m_type(TYPE_ARRAY),
    m_array(new std::vector<Value>{std::move(args)...})
  {}

  void destroy();

  [[noreturn]]
  void type_error(const char* msg) const
  {
    throw TypeError(m_line, msg);
  }

public:
  Value(Value const& other);

  inline Value(Value&& other) :
    m_line(0),
    m_type()
  {
    memcpy(this, &other, sizeof(Value));
    other.m_type = TYPE_NIL;
  }

  inline Value() :
    m_line(0),
    m_type(TYPE_NIL)
  {}

  inline ~Value()
  {
    destroy();
  }

  inline Value& operator=(Value&& other)
  {
    destroy();
    memcpy(this, &other, sizeof(Value));
    other.m_type = TYPE_NIL;
    return *this;
  }

  inline Value& operator=(Value const& other)
  {
    destroy();
    new (this) Value(other);
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
  inline bool is_array() const { return m_type == TYPE_ARRAY; }

  Value const& get_car() const;
  Value const& get_cdr() const;

  Value& get_car();
  Value& get_cdr();

  void set_car(Value&& sexpr);
  void set_cdr(Value&& sexpr);

  void append(Value&& sexpr);

  bool as_bool() const;
  int as_int() const;
  float as_float() const;
  std::string const& as_string() const;
  std::vector<Value> const& as_array() const;

  bool operator==(Value const& other) const;

  std::string str() const;
};

struct Value::Cons
{
  Value car;
  Value cdr;
};

inline
Value::Value(ConsDummy, Value&& car, Value&& cdr) :
  m_line(0),
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

    case Value::TYPE_ARRAY:
      delete m_array;
      break;

    default:
      // atoms don't need deletion
      break;
  }
}

inline
Value::Value(Value const& other) :
  m_line(other.m_line),
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

    case TYPE_ARRAY:
      m_array = new std::vector<Value>(*other.m_array);
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

      case Value::TYPE_ARRAY:
        return *m_array == *rhs.m_array;
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
  if (m_type == TYPE_CONS)
  {
    return m_cons->car;
  }
  else
  {
    type_error("sexp::Value::get_car(): wrong type, expected TYPE_CONS");
  }
}

inline Value const&
Value::get_cdr() const
{
  if (m_type == TYPE_CONS)
  {
  return m_cons->cdr;
  }
  else
  {
    type_error("sexp::Value::get_cdr(): wrong type, expected TYPE_CONS");
  }
}

inline Value&
Value::get_car()
{
  return const_cast<Value&>(static_cast<Value const&>(*this).get_car());
}

inline Value&
Value::get_cdr()
{
  return const_cast<Value&>(static_cast<Value const&>(*this).get_cdr());
}

inline void
Value::set_car(Value&& sexpr)
{
  if (m_type == TYPE_CONS)
  {
    m_cons->car = std::move(sexpr);
  }
  else
  {
    type_error("sexp::Value::set_car(): wrong type, expected TYPE_CONS");
  }
}

inline void
Value::set_cdr(Value&& sexpr)
{
  if (m_type == TYPE_CONS)
  {
    m_cons->cdr = std::move(sexpr);
  }
  else
  {
    type_error("sexp::Value::set_cdr(): wrong type, expected TYPE_CONS");
  }
}

inline void
Value::append(Value&& sexpr)
{
  if (m_type == TYPE_ARRAY)
  {
    m_array->push_back(std::move(sexpr));
  }
  else
  {
    type_error("sexp::Value::append(): wrong type, expected TYPE_ARRAY");
  }
}

inline bool
Value::as_bool() const
{
  if (m_type == TYPE_BOOLEAN)
  {
    return m_bool;
  }
  else
  {
    type_error("sexp::Value::as_bool(): wrong type, expected TYPE_BOOLEAN");
  }
}

inline int
Value::as_int() const
{
  if (m_type == TYPE_INTEGER)
  {
    return m_int;
  }
  else
  {
    type_error("sexp::Value::as_int(): wrong type, expected TYPE_INTEGER");
  }

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
    type_error("sexp::Value::as_float(): wrong type, expected TYPE_INTEGER or TYPE_REAL");
  }
}

inline std::string const&
Value::as_string() const
{
  if (m_type == TYPE_SYMBOL || m_type == TYPE_STRING)
  {
    return *m_string;
  }
  else
  {
    type_error("sexp::Value::as_float(): wrong type, expected TYPE_SYMBOL or TYPE_STRING");
  }
}

inline std::vector<Value> const&
Value::as_array() const
{
  if (m_type == TYPE_ARRAY)
  {
    return *m_array;
  }
  else
  {
    type_error("sexp::Value::as_array(): wrong type, expected TYPE_ARRAY");
  }
}

} // namespace sexp

#endif

/* EOF */
