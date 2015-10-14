//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_SUPERTUX_LISP_PARSER_HPP
#define HEADER_SUPERTUX_LISP_PARSER_HPP

#include <memory>

#include "lisp/lexer.hpp"
#include "lisp/lisp.hpp"
#include "lisp/sexpr.hpp"

namespace lisp {

class Lisp;

class Parser
{
public:
  Parser();
  ~Parser();

  /**
   * Same as parse but reads from a generic std::istream. The sourcename is
   * used for errormessages to indicate the source of the data.
   */
  SExpr parse(std::istream& stream);

private:
  void parse_error(const char* msg) const __attribute__((__noreturn__));
  SExpr read();

private:
  std::unique_ptr<Lexer> lexer;
  Lexer::TokenType token;

private:
  Parser(const Parser&);
  Parser & operator=(const Parser&);
};

} // end of namespace lisp

#endif

/* EOF */
