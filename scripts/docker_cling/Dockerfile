FROM ubuntu:18.04
RUN apt-get update

ARG install_scripts=/install_scripts

ADD install_scripts/install_base.sh $install_scripts/install_base.sh
RUN $install_scripts/install_base.sh

ADD install_scripts/install_miniconda.sh $install_scripts/install_miniconda.sh
RUN $install_scripts/install_miniconda.sh

ADD install_scripts/conda_xeus_cling.sh $install_scripts/conda_xeus_cling.sh
RUN $install_scripts/conda_xeus_cling.sh

ADD install_scripts/conda_opencv.sh $install_scripts/conda_opencv.sh
RUN $install_scripts/conda_opencv.sh

ADD install_scripts/customize_env.sh $install_scripts/customize_env.sh
RUN $install_scripts/customize_env.sh

ADD install_scripts/create_notebook_launcher.sh $install_scripts/create_notebook_launcher.sh
RUN $install_scripts/create_notebook_launcher.sh

RUN /srv/conda/bin/conda install -y -c conda-forge rise

WORKDIR /sources_docker
