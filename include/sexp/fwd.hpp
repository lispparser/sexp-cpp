// SExp - A S-Expression Parser for C++
// Copyright (C) 2021 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_SEXP_FWD_HPP
#define HEADER_SEXP_FWD_HPP

namespace sexp {

class TypeError;
class Lexer;
class Parser;
class Value;
class ListIterator;
class ListAdapter;
class Value;
enum class Type : unsigned char;

} // namespace sexp

#endif

/* EOF */
