import os
import sys
import subprocess

print("ct_compiler_typename_unit_test.py")

cmd = "cmake --build . --target ct_compiler_typename_example 2>&1 | ct_compiler_typename -c"
result = subprocess.run(cmd, capture_output=True, shell=True)
parsed_compiler_output = result.stdout.decode("utf-8")
print("parsed_compiler_output=\n{}".format(parsed_compiler_output))

lines = parsed_compiler_output.split("\n")
lines_with_cpp = [ line for line in lines if "ct_compiler_typename_example.cpp" in line]
if len(lines_with_cpp) != 1:
    print("ct_compiler_typename_unit_test ==> ERROR: count_cpp_file != 1 !!!!!!")
    sys.exit(1)


lines_with_type = [ line for line in lines if "std::map<std::string, int>" in line]
if len(lines_with_type) != 1:
    print("ct_compiler_typename_unit_test ==> ERROR: count_type != 1 !!!!!!")
    sys.exit(1)


print("ct_compiler_typename_unit_test.py ==> OK !!!")
sys.exit(0)
