DIR="$(dirname "$(greadlink -f "$0")")" # readlink on linux (brew install coreutils on mac)
# open http://localhost:8888
docker run --rm -it -p 8888:8888 -v $DIR/../..:/sources_docker cling_xeus /bin/bash  -c "cd /sources_docker && /usr/local/bin/jnote"
