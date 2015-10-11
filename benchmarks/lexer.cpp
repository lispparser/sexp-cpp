#include <benchmark/benchmark.h>

#include <assert.h>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

#include "lisp/lexer.hpp"

static void BM_lexer(benchmark::State& state)
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
      lisp::Lexer lexer(fin);
      while(lexer.getNextToken() != lisp::Lexer::TOKEN_EOF);
    }
  }
}
BENCHMARK(BM_lexer);

BENCHMARK_MAIN()

/* EOF */
