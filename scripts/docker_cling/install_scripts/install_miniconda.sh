set -e
# install miniconda
mkdir -p $install_scripts
curl https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh  -o $install_scripts/miniconda-install.sh
chmod +x $install_scripts/miniconda-install.sh
$install_scripts/miniconda-install.sh -b

# symlink /srv/conda required for path compatibility with mybinder.org
mkdir -p /srv && ln -s /root/miniconda3 /srv/conda

# add /srv/conda/bin to the path
echo "export PATH=\$PATH:/srv/conda/bin" >> /root/.bashrc
echo "export PATH=\$PATH:/srv/conda/bin" >> /root/.zshrc
