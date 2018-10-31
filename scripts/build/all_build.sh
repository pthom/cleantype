this_script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
src_dir=$this_script_dir/../..
$this_script_dir/conan_install.sh
$this_script_dir/cmake_ninja.sh
$this_script_dir/ln_compile_commands.sh

ninja
ninja test
