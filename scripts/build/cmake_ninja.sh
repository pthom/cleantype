DIR="$(dirname "$(readlink -f "$0")")" # readlink on linux (brew install coreutils on mac)
src_dir=$DIR/../..

cmake $src_dir -GNinja
