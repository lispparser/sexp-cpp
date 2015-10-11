#include <benchmark/benchmark.h>

#include <assert.h>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

#include "lisp/lexer.hpp"

static void BM_lexer(benchmark::State& state)
{
  std::ifstream fin("benchmarks/test.lisp");
  if (!fin)
  {
    throw std::runtime_error("failed to open benchmarks/test.lisp");
  }
  else
  {
    std::string str(std::istreambuf_iterator<char>{fin}, std::istreambuf_iterator<char>{});
    while (state.KeepRunning())
    {
      std::istringstream in(str);
      lisp::Lexer lexer(in);
      while(lexer.getNextToken() != lisp::Lexer::TOKEN_EOF);
    }
  }
}
BENCHMARK(BM_lexer);

BENCHMARK_MAIN()

/* EOF */
