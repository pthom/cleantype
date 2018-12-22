DIR="$(dirname "$(greadlink -f "$0")")" # readlink on linux (brew install coreutils on mac)
docker build $DIR -t cling_xeus
