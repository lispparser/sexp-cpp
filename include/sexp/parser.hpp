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

#ifndef HEADER_SEXP_PARSER_HPP
#define HEADER_SEXP_PARSER_HPP

#include <memory>

#include "sexp/lexer.hpp"
#include "sexp/value.hpp"

namespace sexp {

class Parser
{
public:
  static Value from_string(std::string const& str);
  static Value from_stream(std::istream& stream);

public:
  Parser(std::istream& stream);
  ~Parser();

private:
  void parse_error(const char* msg) const __attribute__((__noreturn__));
  Value read();

private:
  std::unique_ptr<Lexer> lexer;
  Lexer::TokenType token;

private:
  Parser(const Parser&);
  Parser & operator=(const Parser&);
};

} // namespace sexp

#endif

/* EOF */
