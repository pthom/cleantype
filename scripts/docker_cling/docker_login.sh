DIR="$(dirname "$(greadlink -f "$0")")" # readlink on linux (brew install coreutils on mac)
# docker run --rm -it -v $DIR/..:/sources_docker cling_xeus /bin/zsh

docker run --rm -it -p 5900:5900 -v $DIR/../..:/sources_docker cling_xeus /bin/zsh
