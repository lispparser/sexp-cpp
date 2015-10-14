#ifndef HEADER_LISPPARSER_LISP_HPP
#define HEADER_LISPPARSER_LISP_HPP

namespace lisp {

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

  Type get_type() const;

  bool is_nil() const;
  bool is_cons() const;
  bool is_symbol() const;
  bool is_string() const;
  bool is_integer() const;
  bool is_real() const;

  SExpr const& car() const;
  SExpr const& cdr() const;

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

SExpr::Type
SExpr::get_type() const
{
  return (m_impl ? m_impl->m_type : SExpr::TYPE_NIL);
}

bool
SExpr::is_nil() const
{
  return m_impl == nullptr;
}

bool
SExpr::is_cons() const
{
  return m_impl && m_impl->m_type == SExpr::TYPE_CONS;
}

bool
SExpr::is_symbol() const
{
  return m_impl && m_impl->m_type == SExpr::TYPE_SYMBOL;
}

bool
SExpr::is_string() const
{
  return m_impl && m_impl->m_type == SExpr::TYPE_STRING;
}

bool
SExpr::is_integer() const
{
  return m_impl && m_impl->m_type == SExpr::TYPE_INTEGER;
}

bool
SExpr::is_real() const
{
  return m_impl && (m_impl->m_type == SExpr::TYPE_REAL ||
                    m_impl->m_type == SExpr::TYPE_INTEGER);
}

SExpr const&
SExpr::car() const
{
  assert(m_impl->m_type == SExpr::TYPE_CONS);
  return m_impl->m_cons.car;
}

SExpr const&
SExpr::cdr() const
{
  assert(m_impl->m_type == SExpr::TYPE_CONS);
  return m_impl->m_cons.cdr;
}

bool
SExpr::as_bool() const
{
  assert(m_impl->m_type == SExpr::TYPE_BOOLEAN);
  return m_impl->m_bool;
}

int
SExpr::as_int() const
{
  assert(m_impl->m_type == SExpr::TYPE_INTEGER);
  return m_impl->m_int;
}

float
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

std::string const&
SExpr::as_string() const
{
  assert(m_impl->m_type == SExpr::TYPE_SYMBOL ||
         m_impl->m_type == SExpr::TYPE_STRING);
  return *m_impl->m_string;
}

} // namespace lisp

#endif

/* EOF */
