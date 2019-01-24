import os
import subprocess


def c(command: str, cwd: str = None) -> str:
    r = subprocess.run(command, cwd=cwd, shell=True,
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    out = r.stdout.decode("utf-8")
    print(out)


code = """
 #include <limits.h>
 int main() {
 }
"""


def compile_code(code):
    with open("f.cpp", "w") as f:
        f.write(code)
    c("clang --std=c++14 -c f.cpp -o a.out -v")


compile_code(code)

# ==>
# In file included from /srv/conda/lib/clang/5.0.0/include/limits.h:37:
# /usr/include/limits.h:145:5: error: function-like macro '__GLIBC_USE' is not defined
# #if __GLIBC_US

# cf https://github.com/QuantStack/xeus-cling/issues/194
# However, it works if I install xtl=0.5.2.
# The only difference seems to be the version of zeu-cling,
# so it's broken with xeus-cling=0.4.10=he860b03_1001 but works with xeus-cling=0.4.10=he860b03_1000
# https://github.com/conda-forge/clangdev-feedstock/pull/45
