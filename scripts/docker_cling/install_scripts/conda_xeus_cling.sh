set -e
export PATH=$PATH:/srv/conda/bin
# install jupyter & xeus.cling
conda install jupyter
conda install -c QuantStack -c conda-forge xeus-cling
