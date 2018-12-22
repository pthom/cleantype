#include "unistd.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <fstream>
#include <stdlib.h>


inline void exec(const std::string & cmd)
{
    std::string cmd_r = cmd + " 2>&1";
    std::cout << cmd_r << std::endl;
    std::array<char, 128> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd_r.c_str(), "r"), pclose);
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        std::cout << buffer.data();
}


inline std::string getcurrendir()
{
    char temp[1024];
    return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}


inline void add_path_to_env(const std::string & path)
{
    std::string env_path = getenv("PATH");
    env_path = env_path + ":" + path;
    setenv("PATH", env_path.c_str(), 1);
}


inline void save_temp_code(const std::string & code)
{
    std::ofstream ofs;
    ofs.open("code.cpp");
    ofs << code;
}


inline void compile_code(const std::string & code)
{
    save_temp_code(code);
    std::string command = "clang++ --std=c++14 -c code.cpp -Iinclude -o a.out 2>&1";
    exec(command);
}

inline void compile_code__extract(const std::string & code)
{
    save_temp_code(code);
    std::string command = "clang++ --std=c++14 -c code.cpp -Iinclude -o a.out 2>&1 | head -3";
    exec(command);
}


inline void compile_code_decipher(const std::string & code)
{
    save_temp_code(code);
    std::string command = "clang++ --std=c++14 -c code.cpp -Iinclude -o a.out 2>&1 | ct_compiler_decipher";
    exec(command);
}

inline void compile_code_decipher__extract(const std::string & code)
{
    save_temp_code(code);
    std::string command = "clang++ --std=c++14 -c code.cpp -Iinclude -o a.out 2>&1 | ct_compiler_decipher | head -3";
    exec(command);
}


inline void make_ct_compiler_decipher()
{
    auto this_dir = getcurrendir();
    chdir("../..");
    unlink("ct_compiler_decipher");
    exec("make");
    add_path_to_env(this_dir + "/../..");
    chdir(this_dir.c_str());
}