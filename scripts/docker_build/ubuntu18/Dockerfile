FROM ubuntu:18.04
RUN apt-get update

ARG install_scripts=/install_scripts

ADD install_scripts/install_base.sh $install_scripts/install_base.sh
RUN $install_scripts/install_base.sh

ADD install_scripts/customize_env.sh $install_scripts/customize_env.sh
RUN $install_scripts/customize_env.sh

ADD install_scripts/install_miniconda.sh $install_scripts/install_miniconda.sh
RUN $install_scripts/install_miniconda.sh

RUN /srv/conda/bin/pip install --upgrade pip && /srv/conda/bin/pip install conan

ADD install_scripts/install_clang.sh $install_scripts/install_clang.sh
RUN $install_scripts/install_clang.sh

WORKDIR /sources_docker
