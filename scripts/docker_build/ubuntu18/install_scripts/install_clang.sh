# cf http://apt.llvm.org/
set -e

apt-get update
apt-get -y install python3-software-properties
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
echo "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-7 main" >> /etc/apt/sources.list
echo "deb-src http://apt.llvm.org/bionic/ llvm-toolchain-bionic-7 main" >> /etc/apt/sources.list
tail /etc/apt/sources.list
apt-get update
apt-get install -y clang-7 lldb-7 lld-7
