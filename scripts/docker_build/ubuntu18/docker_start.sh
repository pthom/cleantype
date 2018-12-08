DIR="$(dirname "$(greadlink -f "$0")")" # readlink on linux (brew install coreutils on mac)
docker run \
 --name u18builder -it -v $DIR/../../..:/sources_docker \
--cap-add=SYS_PTRACE --security-opt seccomp=unconfined --security-opt apparmor=unconfined \
u18build \
/bin/zsh
