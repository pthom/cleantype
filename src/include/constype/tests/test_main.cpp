// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <constype/constype_configuration.hpp>


struct ForceEastConst {
    ForceEastConst() {
        constype::CleanConfiguration::GlobalConfig().force_east_const_ = true;
    }
};

ForceEastConst forceEastConst;
