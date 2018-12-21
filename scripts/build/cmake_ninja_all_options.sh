this_script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
src_dir=$this_script_dir/../..

cmake $src_dir -GNinja -DCLEANTYPE_ALL_OPTIONS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
