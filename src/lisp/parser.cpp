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

#include "lisp/lisp.hpp"
#include "lisp/parser.hpp"

namespace lisp {

Parser::Parser() :
  lexer(),
  token()
{
}

Parser::~Parser()
{
}

const Lisp*
Parser::parse(std::istream& stream)
{
  lexer = std::make_unique<Lexer>(stream);

  token = lexer->getNextToken();

  Lisp* result = new Lisp(Lisp::TYPE_CONS);
  result->v.cons.car = read();
  result->v.cons.cdr = nullptr;

  lexer.reset();

  return result;
}

void
Parser::parse_error(const char* msg) const
{
  std::stringstream emsg;
  emsg << "Parse Error at line " << lexer->getLineNumber()
       << ": " << msg;
  throw std::runtime_error(emsg.str());
}

const Lisp*
Parser::read()
{
  Lisp* result;
  switch(token) {
    case Lexer::TOKEN_EOF: {
      parse_error("Unexpected EOF.");
    }
    case Lexer::TOKEN_CLOSE_PAREN: {
      parse_error("Unexpected ')'.");
    }
    case Lexer::TOKEN_OPEN_PAREN: {
      result = new Lisp(Lisp::TYPE_CONS);

      token = lexer->getNextToken();
      if(token == Lexer::TOKEN_CLOSE_PAREN) {
        result->v.cons.car = nullptr;
        result->v.cons.cdr = nullptr;
      }
      Lisp* cur = result;
      do {
        cur->v.cons.car = read();
        if(token == Lexer::TOKEN_CLOSE_PAREN) {
          cur->v.cons.cdr = nullptr;
          break;
        }
        Lisp *newcur = new Lisp(Lisp::TYPE_CONS);
        cur->v.cons.cdr = newcur;
        cur = newcur;
      } while(1);

      break;
    }
    case Lexer::TOKEN_SYMBOL: {
      result = new Lisp(Lisp::TYPE_SYMBOL);
      size_t len = strlen(lexer->getString()) + 1;
      result->v.string = new char[len];
      memcpy(result->v.string, lexer->getString(), len);
      break;
    }
    case Lexer::TOKEN_STRING: {
      result = new Lisp(Lisp::TYPE_STRING);
      size_t len = strlen(lexer->getString()) + 1;
      result->v.string = new char[len];
      memcpy(result->v.string, lexer->getString(), len);
      break;
    }
    case Lexer::TOKEN_INTEGER:
      result = new Lisp(Lisp::TYPE_INTEGER);
      result->v.integer = atoi(lexer->getString());
      break;
    case Lexer::TOKEN_REAL:
      result = new Lisp(Lisp::TYPE_REAL);
      result->v.real = strtof(lexer->getString(), NULL);
      break;
    case Lexer::TOKEN_TRUE:
      result = new Lisp(Lisp::TYPE_BOOLEAN);
      result->v.boolean = true;
      break;
    case Lexer::TOKEN_FALSE:
      result = new Lisp(Lisp::TYPE_BOOLEAN);
      result->v.boolean = false;
      break;

    default:
      // this should never happen
      result = NULL;
      assert(false);
  }

  token = lexer->getNextToken();
  return result;
}

} // end of namespace lisp

/* EOF */
