this_script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
src_dir=$this_script_dir/../..
build_dir=$(pwd)

cd $src_dir
if [ ! -L compile_commands.json ]; then
  ln -s $build_dir/compile_commands.json .
fi
