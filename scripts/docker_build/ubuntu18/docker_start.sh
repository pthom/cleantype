DIR="$(dirname "$(greadlink -f "$0")")" # readlink on linux (brew install coreutils on mac)
docker run --name u18builder -it -v $DIR/../../..:/sources_docker u18build /bin/zsh
