SExp - A S-Expression Parser for C++
====================================

SExp is an [S-Expression](https://en.wikipedia.org/wiki/S-expression) parser for C++.

Example Code:

    sexp::Value value = sexp::Parser::from_string("(1 2 3 4 5)")
    sexpr::Value const& head = value.car();
    sexpr::Value const& tail = value.cdr();
    if (head.is_integer())
    {
       std::cout << head.as_int() << std::endl;
    }

