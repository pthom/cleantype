# This Makefile is intentionally simple and incomplete
# It is provided as a quick way to build ct_compiler_decipher
# Prefer to use cmake in development mode.

all: ct_compiler_decipher

ct_compiler_decipher: src/tools/ct_compiler_decipher/ct_compiler_decipher.cpp
	$(CXX) -Isrc/include --std=c++14 src/tools/ct_compiler_decipher/ct_compiler_decipher.cpp -o ct_compiler_decipher

clean:
	rm ct_compiler_decipher
