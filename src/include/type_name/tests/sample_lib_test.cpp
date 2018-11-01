#include "doctest.h"
#include "sample_lib/sample_lib.h"

namespace sample_lib
{
  TEST_CASE("testing sample_lib")
  {
    CHECK(42 == 42);
    CHECK(sample_lib::add(6, 5) == 11);
  }
}
