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

#include "sexp/lexer.hpp"

#include <string.h>
#include <sstream>
#include <stdexcept>
#include <stdio.h>

namespace sexp {

Lexer::Lexer(std::istream& newstream, bool use_arrays) :
  stream(newstream),
  m_use_arrays(use_arrays),
  eof(false),
  linenumber(0),
  bufend(),
  bufpos(),
  c(),
  token_string()
{
  // trigger a refill of the buffer
  bufpos = nullptr;
  bufend = nullptr;
  nextChar();
}

Lexer::~Lexer()
{
}

void
Lexer::nextChar()
{
  if(bufpos >= bufend || (bufpos == nullptr && bufend == nullptr) /* Initial refill trigger */) {
    if(eof) {
      c = EOF;
      return;
    }
    stream.read(buffer, BUFFER_SIZE);
    std::streamsize bytes_read = stream.gcount();

    bufpos = buffer;
    bufend = buffer + bytes_read;

    // the following is a hack that appends an additional ' ' at the end of
    // the file to avoid problems when parsing symbols/elements and a sudden
    // EOF. This is faster than relying on unget and IMO also nicer.
    if(bytes_read == 0 || stream.eof()) {
      eof = true;
      *bufend = ' ';
      ++bufend;
    }
  }

  if(bufpos == nullptr)
    return;

  c = *bufpos++;
  if(c == '\n')
    ++linenumber;
}

void
Lexer::addChar()
{
  token_string += static_cast<char>(c);
  nextChar();
}

Lexer::TokenType
Lexer::getNextToken()
{
  static const char* delims = "\"();";

  while(isspace(c)) {
    nextChar();
  }

  token_string.clear();

  switch(c)
  {
    case ';': // comment
      while(c != '\n') {
        nextChar();
      }
      return getNextToken(); // and again

    case '(':
      nextChar();
      if (m_use_arrays)
      {
        return TOKEN_ARRAY_START;
      }
      else
      {
        return TOKEN_OPEN_PAREN;
      }
    case ')':
      nextChar();
      return TOKEN_CLOSE_PAREN;

    case '"': {  // string
      int startline = linenumber;
      while(1) {
        nextChar();
        switch(c) {
          case '"':
            nextChar();
            goto string_finished;
          case '\r':
            continue;
          case '\n':
            break;
          case '\\':
            nextChar();
            switch(c) {
              case 'n':
                c = '\n';
                break;
              case 't':
                c = '\t';
                break;
            }
            break;
          case EOF: {
            std::stringstream msg;
            msg << "Parse error in line " << startline << ": "
                << "EOF while parsing string.";
            throw std::runtime_error(msg.str());
          }
          default:
            break;
        }
        token_string += static_cast<char>(c);
      }
      string_finished:
      return TOKEN_STRING;
    }
    case '#': // constant
      nextChar();

      if (c == '(')
      {
        nextChar();
        return TOKEN_ARRAY_START;
      }
      else
      {
        while(isalnum(c) || c == '_') {
          addChar();
        }

        if(token_string == "t")
        {
          return TOKEN_TRUE;
        }
        else if(token_string == "f")
        {
          return TOKEN_FALSE;
        }
        else
        {
          // we only handle #t and #f constants at the moment...
          std::stringstream msg;
          msg << "Parse Error in line " << linenumber << ": "
              << "Unknown constant '" << token_string << "'.";
          throw std::runtime_error(msg.str());
        }
      }

    case EOF:
      return TOKEN_EOF;

    default:
      if(isdigit(c) || c == '-' || c == '.')
      {
        bool have_nondigits = false;
        bool have_digits = false;
        int have_floating_point = 0;

        do
        {
          if (isdigit(c))
          {
            have_digits = true;
          }
          else if (c == '.')
          {
            ++have_floating_point;
          }
          else if (isalnum(c) || c == '_')
          {
            have_nondigits = true;
          }

          addChar();
        }
        while(!isspace(c) && !strchr(delims, c));

        // no nextChar

        if (token_string == ".")
        {
          return TOKEN_DOT;
        }
        else if (have_nondigits || !have_digits || have_floating_point > 1)
        {
          return TOKEN_SYMBOL;
        }
        else if (have_floating_point == 1)
        {
          return TOKEN_REAL;
        }
        else
        {
          return TOKEN_INTEGER;
        }
      }
      else
      {
        do {
          addChar();
        } while(!isspace(c) && !strchr(delims, c));

        // no nextChar

        return TOKEN_SYMBOL;
      }
  }
}

int
Lexer::getInteger() const
{
  int result = 0;
  if (token_string[0] == '-')
  {
    result += token_string[1] - '0';
    for(size_t i = 2; i < token_string.size(); ++i)
    {
      result *= 10;
      result += token_string[i] - '0';
    }
    return -result;
  }
  else
  {
    result += token_string[0] - '0';
    for(size_t i = 1; i < token_string.size(); ++i)
    {
      result *= 10;
      result += token_string[i] - '0';
    }
    return result;
  }
}

float
Lexer::getReal() const
{
  float result = 0.0f;
  float sign = 1.0f;

  // sign
  size_t i = 0;
  if (token_string[i] == '-')
  {
    i += 1;
    sign = -1;
  }

  // integer part
  for(; token_string[i] != '.'; ++i)
  {
    result *= 10.0f;
    result += static_cast<float>(token_string[i] - '0');
  }

  i += 1;

  // fractional part
  float fraction = 0.1f;
  for(; i < token_string.size(); ++i)
  {
    result += static_cast<float>(token_string[i] - '0') * fraction;
    fraction *= 0.1f;
  }

  return sign * result;
}


} // namespace sexp

/* EOF */
