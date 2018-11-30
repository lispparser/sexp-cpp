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

  union Data
  {
    inline Data() {}
    inline Data(bool v) : m_bool(v) {}
    inline Data(int v) : m_int(v) {}
    inline Data(float v) : m_float(v) {}
    inline Data(std::string* v) : m_string(v) {}
    inline Data(Cons* v) : m_cons(v) {}
    inline Data(std::vector<Value>* v) : m_array(v) {}

    bool m_bool;
    int m_int;
    float m_float;

    std::string* m_string;
    Cons* m_cons;
    std::vector<Value>* m_array;
  } m_data;

  struct BooleanTag {};
  struct IntegerTag {};
  struct RealTag {};
  struct StringTag {};
  struct SymbolTag {};
  struct ConsTag {};
  struct ArrayTag {};

public:
  /** Returns a reference to a nil value for use in functions that
      return a reference and need to return a nil value */
  static Value const& nil_ref() { static Value const s_nil; return s_nil; }
  static Value nil() { return Value(); }
  static Value boolean(bool v) { return Value(BooleanTag(), v); }
  static Value integer(int v) { return Value(IntegerTag(), v); }
  static Value real(float v) { return Value(RealTag(), v); }
  static Value string(std::string const& v) { return Value(StringTag(), v); }
  static Value symbol(std::string const& v) { return Value(SymbolTag(), v); }
  static Value cons(Value&& car, Value&& cdr) { return Value(ConsTag(), std::move(car), std::move(cdr)); }
  static Value cons() { return Value(ConsTag(), Value::nil(), Value::nil()); }

  static Value array(std::vector<Value> arr) { return Value(ArrayTag(), std::move(arr)); }
  template<typename... Args>
  static Value array(Args&&... args) { return Value(ArrayTag(), std::move(args)...); }

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
  inline explicit Value(BooleanTag, bool value) : m_line(0), m_type(TYPE_BOOLEAN), m_data(value) {}
  inline explicit Value(IntegerTag, int value) : m_line(0), m_type(TYPE_INTEGER), m_data(value) {}
  inline explicit Value(RealTag, float value) : m_line(0), m_type(TYPE_REAL), m_data(value) {}
  inline Value(StringTag, std::string const& value) :
    m_line(0),
    m_type(TYPE_STRING),
    m_data(new std::string(value))
  {}
  inline Value(SymbolTag, std::string const& value) :
    m_line(0),
    m_type(TYPE_SYMBOL),
    m_data(new std::string(value))
  {}
  inline Value(ConsTag, Value&& car, Value&& cdr);
  inline Value(ArrayTag, std::vector<Value> arr) :
    m_line(0),
    m_type(TYPE_ARRAY),
    m_data(new std::vector<Value>(std::move(arr)))
  {}
  template<typename... Args>
  inline Value(ArrayTag, Args&&... args) :
    m_line(0),
    m_type(TYPE_ARRAY),
    m_data(new std::vector<Value>{std::move(args)...})
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
    m_line(other.m_line),
    m_type(other.m_type),
    m_data(other.m_data)
  {
    other.m_type = TYPE_NIL;
  }

  inline Value() :
    m_line(0),
    m_type(TYPE_NIL),
    m_data()
  {}

  inline ~Value()
  {
    destroy();
  }

  inline Value& operator=(Value&& other)
  {
    destroy();

    m_line = other.m_line;
    m_type = other.m_type;
    m_data = other.m_data;

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
Value::Value(ConsTag, Value&& car, Value&& cdr) :
  m_line(0),
  m_type(TYPE_CONS),
  m_data{.m_cons = new Cons{std::move(car), std::move(cdr)}}
{}

inline void
Value::destroy()
{
  switch(m_type)
  {
    case Value::TYPE_STRING:
    case Value::TYPE_SYMBOL:
      delete m_data.m_string;
      break;

    case Value::TYPE_CONS:
      delete m_data.m_cons;
      break;

    case Value::TYPE_ARRAY:
      delete m_data.m_array;
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
      m_data.m_bool = other.m_data.m_bool;
      break;

    case TYPE_INTEGER:
      m_data.m_int = other.m_data.m_int;
      break;

    case TYPE_REAL:
      m_data.m_float = other.m_data.m_float;
      break;

    case TYPE_STRING:
    case TYPE_SYMBOL:
      m_data.m_string = new std::string(*other.m_data.m_string);
      break;

    case TYPE_CONS:
      m_data.m_cons = new Cons(*other.m_data.m_cons);
      break;

    case TYPE_ARRAY:
      m_data.m_array = new std::vector<Value>(*other.m_data.m_array);
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
        return m_data.m_bool == rhs.m_data.m_bool;

      case Value::TYPE_INTEGER:
        return m_data.m_int == rhs.m_data.m_int;

      case Value::TYPE_REAL:
        return m_data.m_float == rhs.m_data.m_float;

      case Value::TYPE_STRING:
      case Value::TYPE_SYMBOL:
        return *m_data.m_string == *rhs.m_data.m_string;

      case Value::TYPE_CONS:
        return (m_data.m_cons->car == rhs.m_data.m_cons->car &&
                m_data.m_cons->cdr == rhs.m_data.m_cons->cdr);

      case Value::TYPE_ARRAY:
        return *m_data.m_array == *rhs.m_data.m_array;
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
    return m_data.m_cons->car;
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
  return m_data.m_cons->cdr;
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
    m_data.m_cons->car = std::move(sexpr);
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
    m_data.m_cons->cdr = std::move(sexpr);
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
    m_data.m_array->push_back(std::move(sexpr));
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
    return m_data.m_bool;
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
    return m_data.m_int;
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
    return m_data.m_float;
  }
  else if (m_type == TYPE_INTEGER)
  {
    return static_cast<float>(m_data.m_int);
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
    return *m_data.m_string;
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
    return *m_data.m_array;
  }
  else
  {
    type_error("sexp::Value::as_array(): wrong type, expected TYPE_ARRAY");
  }
}

} // namespace sexp

#endif

/* EOF */
