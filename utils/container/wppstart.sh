#------------------------------------
# Dependencies and Tools
# Run Wakaama LwM2M localy
#
# Made for Sinai RnD
# Made by  Viacheslav Diachok
#-------------------------------------

# chmod +x wppstart.sh
# ./wppstart.sh <name_exec_file> (ex.: ./wppstart.sh ./../../_build/examples/WppExample )

#!/bin/bash

if [ -x "$(command -v docker)" ]; then
    echo "Running docker"
    # command
else
    echo "Install docker"
    sudo apt update
    sudo apt install docker.io -y
fi

# download image from dockerhub
sudo docker pull slavkodo/wakaama_pp_env:latest > /dev/null 2>&1

# run docker container and executable(bin) file with WppExample
sudo docker run --name wakaama_pp -it -v $1:/ slavkodo/wakaama_pp_env 
$1

# remove container after success running (if your script says: wakaama_pp exists - run again ./wppstart.sh <name_exec_file>)                                                                                                                          
sudo docker rm -f wakaama_pp

