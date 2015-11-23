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

#ifndef HEADER_SEXP_LEXER_HPP
#define HEADER_SEXP_LEXER_HPP

#include <istream>

namespace sexp {

class Lexer
{
public:
  enum TokenType {
    TOKEN_EOF,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_DOT,
    TOKEN_SYMBOL,
    TOKEN_STRING,
    TOKEN_INTEGER,
    TOKEN_REAL,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_ARRAY_START
  };

  Lexer(std::istream& stream, bool use_arrays = false);
  ~Lexer();

  TokenType getNextToken();
  std::string const& getString() const { return token_string; }
  int getInteger() const;
  float getReal() const;
  int getLineNumber() const
  { return linenumber; }

private:
  static const int MAX_TOKEN_LENGTH = 16384;
  static const int BUFFER_SIZE = 16384;

  inline void nextChar();
  inline void addChar();

private:
  std::istream& stream;
  bool m_use_arrays;
  bool eof;
  int linenumber;
  char buffer[BUFFER_SIZE+1];
  char* bufend;
  char* bufpos;
  int  c;
  std::string token_string;

private:
  Lexer(const Lexer&);
  Lexer & operator=(const Lexer&);
};

} // namespace sexp

#endif

/* EOF */
