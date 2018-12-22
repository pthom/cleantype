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


void exec_log_cout(const char* cmd)
{
    std::array<char, 128> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        std::cout << buffer.data();
}

void exec_log_cout(const std::string & cmd)
{
    exec_log_cout(cmd.c_str());
}


std::string getcurrendir()
{
    char temp[1024];
    return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}

std::string topdir = getcurrendir();

void choose_clang()
{
    setenv("CC", "clang", 1);
    setenv("CXX", "clang", 1);
}

void go_to_build_dir()
{
    exec_log_cout("mkdir build");
    chdir("build");
}

void go_back_to_top_dir()
{
    chdir(topdir.c_str());
}


void install_conan()
{
    std::cout << "installing conan (pip install conan)... " << std::endl;
    exec_log_cout("pip install conan");
    exec_log_cout("conan user");
    std::cout << " Done" << std::endl;
}

void install_conan_deps()
{
    std::cout << "install conan deps (conan install ../../..)... " << std::endl;
    exec_log_cout("conan install ../../.. -s compiler=clang 2>&1");
    std::cout << " Done" << std::endl;
}

void  run_cmake()
{
    std::cout << "running cmake... " << std::endl;
    exec_log_cout("cmake ../../.. -DCLEANTYPE_ALL_OPTIONS=ON -DCMAKE_INSTALL_PREFIX=install/ 2>&1");
    std::cout << " Done\n" << std::endl;
}

void build_and_install()
{
    exec_log_cout("cmake --build . --target install 2>&1");

    std::string path = getenv("PATH");
    path = path + ":" + topdir + "/build/install/bin";
    setenv("PATH", path.c_str(), 1);
}

void build_ct_tools()
{
    choose_clang();
    go_to_build_dir();
    install_conan();
    install_conan_deps();
    run_cmake();
    build_and_install();
    go_back_to_top_dir();
}

void save_temp_code(const std::string & code)
{
    std::ofstream ofs;
    ofs.open("code.cpp");
    ofs << code;
}

void compile_code(const std::string & code)
{
    save_temp_code(code);
    std::string command = "clang++ --std=c++14 -c code.cpp -Iinclude -o a.out 2>&1";
    std::cout << command << std::endl;
    exec_log_cout(command);
}

void compile_code_decipher(const std::string & code)
{
    save_temp_code(code);
    std::string command = "clang++ --std=c++14 -c code.cpp -Iinclude -o a.out 2>&1 | ct_compiler_decipher";
    std::cout << command << std::endl;
    exec_log_cout(command);
}

void compile_code_typename(const std::string & code)
{
    save_temp_code(code);
    std::string command = "clang++ --std=c++14 -c code.cpp -Iinclude -o a.out 2>&1 | ct_compiler_typename -c 2>&1";
    std::cout << command << std::endl;
    exec_log_cout(command);
}
