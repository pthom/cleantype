// Special macro to change the include path (this is required for the binder demo)
#pragma cling add_include_path("../include")
#include <cleantype/cleantype.hpp>

#include <vector>
#include <map>
#include <array>
//#include <future>
#include <numeric>
//#include <fplus/fplus.hpp>
//#include <range/v3/all.hpp>

// This macro is used later in this manual in order to display commands and their results
#define run_show(...)                   \
{                                       \
    std::cout << #__VA_ARGS__ << "\n";  \
    std::cout << __VA_ARGS__ << "\n\n"; \
}
