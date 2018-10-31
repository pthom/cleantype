this_script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
src_dir=$this_script_dir/../..
conan install $src_dir --build=missing
