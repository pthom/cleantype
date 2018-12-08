# cf http://apt.llvm.org/
set -e
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
apt-get -y install software-properties-common

# add-apt-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-trusty-7 main"
# echo "deb http://apt.llvm.org/bionic/ llvm-toolchain-trusty-7 main" >> /etc/apt/sources.list.d/clang.list

# apt-get update

# apt-get install -y clang-6.0 clang-tools-6.0 clang-6.0-doc libclang-common-6.0-dev libclang-6.0-dev libclang1-6.0 clang-format-6.0 python-clang-6.0
# apt-get install clang-7 clang-tools-7 clang-7-doc libclang-common-7-dev libclang-7-dev libclang1-7 clang-format-7 python-clang-7
