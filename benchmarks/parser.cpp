#include <benchmark/benchmark.h>

#include <assert.h>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

#include "sexp/parser.hpp"

static void BM_parser(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    std::ifstream fin("benchmarks/test.lisp");
    if (!fin)
    {
      throw std::runtime_error("failed to open benchmarks/test.lisp");
    }
    else
    {
      sexp::SExpr sexpr = sexp::Parser::from_stream(fin);
    }
  }
}
BENCHMARK(BM_parser);

BENCHMARK_MAIN()

/* EOF */
