#------------------------------------
# Dependencies and Tools
# Run Wakaama LwM2M localy
#
# Made for Sinai RnD
# Made by  Viacheslav Diachok
#-------------------------------------

#!/bin/bash

CLANG_VERSION=14

CLANG="clang-$CLANG_VERSION"
CLANG_KIT_PATH="/usr/bin/clang"

CLANGPP="clang++-$CLANG_VERSION"
CLANGPP_KIT_PATH="/usr/bin/clang++"

tools="\
autoconf \
automake \
libtool \
pkg-config \
default-jre \
libcunit1 \
libcunit1-doc \
libcunit1-dev \
ninja-build \
python3-pip \
gcovr \
git \
build-essential \
clang-format \
clang-format-$CLANG_VERSION \
clang-tools-$CLANG_VERSION \
lldb-$CLANG_VERSION \
$CLANG \
cppcheck \
llvm \
libcurl4-openssl-dev \
cmake
g++-12"

ptools="\
cmake-format \
gitlint \
pylint \
pexpect \
pytest"

for x in $tools; do
	sudo apt-get install $x -y
	if [ $? -ne 0 ]; then
		echo "Error installing $x."
		exit 1
	fi
done
        
for x in $ptools; do
    pip3 install $x
    if [ $? -ne 0 ]; then
        echo "Error installing $x with pip."
        exit 1
    fi
done

sudo snap install cmake --classic # version 3.26

# Setup clang kit path
echo -e "\nSetup clang kit"
CLANG_PATH=$(which $CLANG)
if [ -z "$CLANG_PATH" ]; then
	echo "Error: ${CLANG} not found in PATH."
	exit 1
fi
if [ ! -e "$CLANG_KIT_PATH" ]; then
	# Create the symbolic link
	sudo ln -s $CLANG_PATH $CLANG_KIT_PATH
	echo "$CLANG_KIT_PATH symbolic link created successfully."
fi

CLANGPP_PATH=$(which $CLANGPP)
if [ -z "$CLANGPP_PATH" ]; then
	echo "Error: ${CLANGPP} not found in PATH."
	exit 1
fi
if [ ! -e "$CLANGPP_KIT_PATH" ]; then
	# Create the symbolic link
	sudo ln -s $CLANGPP_PATH $CLANGPP_KIT_PATH
	echo "$CLANGPP_KIT_PATH symbolic link created successfully."
fi
echo -e "Setup clang kit DONE"

echo -e "\nSetup Doxygen"
./docs/doxygen_setup.sh
echo -e "Setup Doxygen DONE"

# TODO: Move docs generation to cmake file, with converting to .pdf
echo -e "\nGenerating Doxygen documentation"
cd docs
doxygen Doxygen
xdg-open html/index.html
cd ..
echo -e "Generating Doxygen documentation DONE"

echo -e "\nSetup environment DONE"

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
# Repo for Wpp
# cd ${HOME}/Desktop
# git clone --recurse-submodules git@github.com:<repo>
#
# Install Eclipse IDE:
# https://www.eclipse.org/downloads/packages/installer
#
