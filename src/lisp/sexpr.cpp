#include "lisp/sexpr.hpp"

#include <sstream>

#include "lisp/io.hpp"

namespace lisp {

std::string
SExpr::str() const
{
  std::ostringstream os;
  os << *this;
  return os.str();
}

} // namespace lisp

/* EOF */
