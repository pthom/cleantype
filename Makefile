# This Makefile is intentionally simple and incomplete
# It is provided as a quick way to build ct_compiler_decipher
# Prefer to use cmake in development mode.
ct_compiler_decipher:
	$(CXX) -Isrc/include --std=c++14 src/tools/ct_compiler_decipher/ct_compiler_decipher.cpp -o ct_compiler_decipher
all:
	ct_compiler_decipher
