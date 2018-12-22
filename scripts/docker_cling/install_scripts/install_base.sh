set -e

apt-get install -y build-essential python3 python3-pip zsh curl wget cmake ninja-build git tree htop nano vim

# install zsh & oh-my-zsh (so that we have nice colors)
apt-get install -y zsh wget git
wget https://github.com/robbyrussell/oh-my-zsh/raw/master/tools/install.sh -O - | zsh || true
