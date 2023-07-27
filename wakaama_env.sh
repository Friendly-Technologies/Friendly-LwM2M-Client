#------------------------------------
# Dependencies and Tools
# Run Wakaama LwM2M localy
#
# Made for Sinai RnD
# Made by  Viacheslav Diachok
#-------------------------------------

#!/bin/bash

tools="\
default-jre \
libcunit1 \
libcunit1-doc \
libcunit1-dev \
ninja-build \
python3-pip \
gcovr \
git \
Build-essential \
clang-format \
clang-format-14 \
clang-tools-14 \
clang-6.0"

ptools="\
cmake-format \
gitlint \
pylint \
pexpect \
pytest"

for x in $tools; do
sudo apt-get install $x -y
done
        
for x in $ptools; do
pip3 install $x
done

sudo snap install cmake --classic # version 3.26

# wget https://launchpad.net/ubuntu/+archive/primary/+sourcefiles/cmake/3.25.1-1ubuntu2/cmake_3.25.1.orig.tar.gz
# tar -xvzf cmake_3.25.1.orig.tar.gz
# cd cmake-3.25.1
# ./configure
# make
# sudo make install
# cd ../
# rm -r cmake-3.25.1
# rm -r cmake_3.25.1.orig.tar.gz

#export PATH=~/usr/bin/cmake:$PATH # if you have problem with path 

# wget https://github.com/Kitware/CMake/releases/download/v3.26.4/cmake-3.26.4.tar.gz
# tar -xvzf cmake-3.26.4.tar.gz
# cd cmake-3.26.4
# ./configure
# make
# sudo make install
# cd ../
# sudo rm -r cmake-3.26.4
# sudo rm -r cmake-3.26.4.tar.gz

#
# Repo for Wakaama++
# cd ${HOME}/Desktop
# git clone --recurse-submodules git@github.com:sinai-io/2305-WakaamaPlus.git
#
# Install Eclipse IDE:
# https://www.eclipse.org/downloads/packages/installer
#