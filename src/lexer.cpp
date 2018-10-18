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
#include <string>
#include <sstream>
#include <stdexcept>
#include <stdio.h>

namespace sexp {

Lexer::Lexer(std::istream& newstream, bool use_arrays) :
  m_stream(newstream),
  m_use_arrays(use_arrays),
  m_eof(false),
  m_linenumber(0),
  m_bufend(),
  m_bufpos(),
  m_c(),
  m_token_string()
{
  // trigger a refill of the buffer
  m_bufpos = nullptr;
  m_bufend = nullptr;
  next_char();
}

Lexer::~Lexer()
{
}

void
Lexer::next_char()
{
  if (m_bufpos >= m_bufend || (m_bufpos == nullptr && m_bufend == nullptr) /* Initial refill trigger */) {
    if (m_eof) {
      m_c = EOF;
      return;
    }
    m_stream.read(m_buffer, BUFFER_SIZE);
    std::streamsize bytes_read = m_stream.gcount();

    m_bufpos = m_buffer;
    m_bufend = m_buffer + bytes_read;

    // the following is a hack that appends an additional ' ' at the end of
    // the file to avoid problems when parsing symbols/elements and a sudden
    // EOF. This is faster than relying on unget and IMO also nicer.
    if (bytes_read == 0 || m_stream.eof()) {
      m_eof = true;
      *m_bufend = ' ';
      ++m_bufend;
    }
  }

  if (m_bufpos == nullptr)
    return;

  m_c = *m_bufpos++;
  if (m_c == '\n')
    ++m_linenumber;
}

void
Lexer::add_char()
{
  m_token_string += static_cast<char>(m_c);
  next_char();
}

Lexer::TokenType
Lexer::get_next_token()
{
  static const char* delims = "\"();";

  while(isspace(m_c)) {
    next_char();
  }

  m_token_string.clear();

  switch(m_c)
  {
    case ';': // comment
      while(m_c != '\n') {
        next_char();
      }
      return get_next_token(); // and again

    case '(':
      next_char();
      if (m_use_arrays)
      {
        return TOKEN_ARRAY_START;
      }
      else
      {
        return TOKEN_OPEN_PAREN;
      }
    case ')':
      next_char();
      return TOKEN_CLOSE_PAREN;

    case '"': {  // string
      int startline = m_linenumber;
      while(1) {
        next_char();
        switch(m_c) {
          case '"':
            next_char();
            goto string_finished;
          case '\r':
            continue;
          case '\n':
            break;
          case '\\':
            next_char();
            switch(m_c) {
              case 'n':
                m_c = '\n';
                break;
              case 't':
                m_c = '\t';
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
        m_token_string += static_cast<char>(m_c);
      }
      string_finished:
      return TOKEN_STRING;
    }
    case '#': // constant
      next_char();

      if (m_c == '(')
      {
        next_char();
        return TOKEN_ARRAY_START;
      }
      else
      {
        while(isalnum(m_c) || m_c == '_') {
          add_char();
        }

        if (m_token_string == "t")
        {
          return TOKEN_TRUE;
        }
        else if (m_token_string == "f")
        {
          return TOKEN_FALSE;
        }
        else
        {
          // we only handle #t and #f constants at the moment...
          std::stringstream msg;
          msg << "Parse Error in line " << m_linenumber << ": "
              << "Unknown constant '" << m_token_string << "'.";
          throw std::runtime_error(msg.str());
        }
      }

    case EOF:
      return TOKEN_EOF;

    default:
      if (isdigit(m_c) || m_c == '-' || m_c == '.')
      {
        bool have_nondigits = false;
        bool have_digits = false;
        int have_floating_point = 0;

        do
        {
          if (isdigit(m_c))
          {
            have_digits = true;
          }
          else if (m_c == '.')
          {
            ++have_floating_point;
          }
          else if (isalnum(m_c) || m_c == '_')
          {
            have_nondigits = true;
          }

          add_char();
        }
        while(!isspace(m_c) && !strchr(delims, m_c));

        // no next_char

        if (m_token_string == ".")
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
          add_char();
        } while(!isspace(m_c) && !strchr(delims, m_c));

        // no next_char

        return TOKEN_SYMBOL;
      }
  }
}

int
Lexer::get_integer() const
{
  return std::stoi(m_token_string);
}

float
Lexer::get_real() const
{
  float result = 0.0f;
  float sign = 1.0f;

  // sign
  size_t i = 0;
  if (m_token_string[i] == '-')
  {
    i += 1;
    sign = -1;
  }

  // integer part
  for(; m_token_string[i] != '.'; ++i)
  {
    result *= 10.0f;
    result += static_cast<float>(m_token_string[i] - '0');
  }

  i += 1;

  // fractional part
  float fraction = 0.1f;
  for(; i < m_token_string.size(); ++i)
  {
    result += static_cast<float>(m_token_string[i] - '0') * fraction;
    fraction *= 0.1f;
  }

  return sign * result;
}


} // namespace sexp

/* EOF */
