Linux & OSX :
[![Travis](https://travis-ci.org/pthom/cpp_skeleton.svg?branch=master)](https://travis-ci.org/pthom/cpp_skeleton)
MSVC :
[![AppVeyor](https://ci.appveyor.com/api/projects/status/github/pthom/cpp_skeleton)](https://ci.appveyor.com/project/pthom/cpp-skeleton)


# cpp_skeleton

A C++ project skeleton, that includes :

- conanfile  and tools
- travis and appveyor support (with conan)
- unit tests using doctests and cmake_register_test
- ccache auto-detection
- pre-populated folder structure

````
.
├── AUTHORS
├── .gitignore               Some good defaults
├── .gitmodules
├── .travis.yml               Travis configured with conan, for Clang 6 & 6, gcc 4->8, OSX
├── CMakeLists.txt
├── LICENSE
├── README.md
├── appveyor.yml              Appveyor configured for MSVC 2015 & 2017 64 bits
├── build/
├── cmake/
│   ├── cmake_init/
│   └── cmake_registertest/   Utility to register tests (submodule)
├── docs/
├── scripts/
│   └── build/                Some scripts to all build & install third parties
│       ├── all_build.sh*
│       ├── cmake_ninja.sh*
│       ├── conan_install.sh*
│       └── ln_compile_commands.sh*
├── src/
│   ├── libs/
│   │   └── sample_lib/
│   │       └── tests/         The test are grouped per-library, as they should be
|   └── main/
└── third_party/
    ├── FunctionalPlus/        Header only third party (submodule)
    └── doctest/               Header only third party (submodule)
````
