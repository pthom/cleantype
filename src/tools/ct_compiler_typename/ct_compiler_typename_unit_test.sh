
echo "src/tools/ct_compiler_decipher/ct_compiler_typename_unit_test.sh"

parsed_compiler_output=$(cmake --build . --target ct_compiler_typename_example 2>&1 | ./ct_compiler_typename -c)
echo "parsed_compiler_output=\n$parsed_compiler_output"


# "Searching for lines corresponding to the correct cpp file in the parsed output : should be 1"
count_cpp_file=$(echo $parsed_compiler_output | grep ct_compiler_typename_example.cpp | wc -l | sed s/\ //g )
echo "count_cpp_file=$count_cpp_file"
if [[ $count_cpp_file -ne "1" ]]; then
    echo "ct_compiler_typename_unit_test ==> ERROR: count_cpp_file != 1 !!!!!!"
    exit 1
fi


# "Searching for lines containing the correct type file in the parsed output : should be 1"
count_type=$(echo $parsed_compiler_output | grep "std::map<std::string, int>" | wc -l | sed s/\ //g )
echo "count_type=$count_type"
if [[ $count_type -ne "1" ]]; then
    echo "ct_compiler_typename_unit_test ==> ERROR: count_type != 1 !!!!!!"
    exit 1
fi

exit 0
