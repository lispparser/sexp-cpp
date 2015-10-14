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

#include <sstream>
#include <stdexcept>
#include <string.h>
#include <iostream>

#include "lisp/parser.hpp"

namespace lisp {

SExpr
Parser::from_string(std::string const& str)
{
  std::istringstream is(str);
  Parser parser(is);
  return parser.read();
}

SExpr
Parser::from_stream(std::istream& stream)
{
  Parser parser(stream);
  return parser.read();
}

Parser::Parser(std::istream& stream) :
  lexer(std::make_unique<Lexer>(stream)),
  token(lexer->getNextToken())
{
}

Parser::~Parser()
{
}

void
Parser::parse_error(const char* msg) const
{
  std::stringstream emsg;
  emsg << "Parse Error at line " << lexer->getLineNumber()
       << ": " << msg;
  throw std::runtime_error(emsg.str());
}

SExpr
Parser::read()
{
  SExpr result;

  switch(token)
  {
    case Lexer::TOKEN_EOF:
      parse_error("Unexpected EOF.");
      break;

    case Lexer::TOKEN_CLOSE_PAREN:
      parse_error("Unexpected ')'.");
      break;

    case Lexer::TOKEN_OPEN_PAREN:
      token = lexer->getNextToken();
      if(token == Lexer::TOKEN_CLOSE_PAREN)
      {
        result = SExpr::nil();
      }
      else
      {
        result = SExpr::cons(read(), SExpr::nil());
        SExpr* cur = &result;
        while(token != Lexer::TOKEN_CLOSE_PAREN)
        {
          cur->set_cdr(SExpr::cons(read(), SExpr::nil()));
          cur = &cur->get_cdr();
        }
      }
      break;

    case Lexer::TOKEN_SYMBOL:
      result = SExpr::symbol(lexer->getString());
      break;

    case Lexer::TOKEN_STRING:
      result = SExpr::string(lexer->getString());
      break;

    case Lexer::TOKEN_INTEGER:
      result = SExpr::integer(stoi(lexer->getString()));
      break;

    case Lexer::TOKEN_REAL:
      result = SExpr::real(stof(lexer->getString()));
      break;

    case Lexer::TOKEN_TRUE:
      result = SExpr::boolean(true);
      break;

    case Lexer::TOKEN_FALSE:
      result = SExpr::boolean(false);
      break;

    default:
      assert(false && "this should never happen");
      break;
  }

  token = lexer->getNextToken();
  return result;
}

} // end of namespace lisp

/* EOF */
