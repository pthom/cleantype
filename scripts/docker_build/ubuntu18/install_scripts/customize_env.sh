
# Prompt with a [DOCKER] prefix
echo "
export PROMPT='[DOCKER]\${ret_status} %{\$fg[cyan]%}%~%{\$reset_color%} > '
" >> /root/.zshrc
#
echo "export LC_ALL=C.UTF-8" >> /root/.zshrc
echo "export LC_ALL=C.UTF-8" >> /root/.bashrc
echo "export LANG=C.UTF-8" >> /root/.zshrc
echo "export LANG=C.UTF-8" >> /root/.bashrc
