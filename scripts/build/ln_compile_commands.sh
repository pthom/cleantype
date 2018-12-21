DIR="$(dirname "$(readlink -f "$0")")" # readlink on linux (brew install coreutils on mac)
src_dir=$DIR/../..
build_dir=$(pwd)

cd $src_dir
if [ ! -L compile_commands.json ]; then
  ln -s $build_dir/compile_commands.json .
fi
