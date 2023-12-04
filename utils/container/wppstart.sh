#------------------------------------
# Dependencies and Tools
# Run Wakaama LwM2M localy
#
# Made for Sinai RnD
# Made by  Viacheslav Diachok
#-------------------------------------

# chmod +x wppstart.sh
# ./wppstart.sh <name_exec_file> (ex.: ./wppstart.sh ./../../_build/examples/WakaamaPlusExample )

#!/bin/bash

# install docker
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin -y > /dev/null 2>&1

# download image from dockerhub
docker pull slavkodo/wakaama_pp_env:latest > /dev/null 2>&1

# run docker container and executable(bin) file with WakaamaPlusPlus
docker run --name wakaama_pp -it -v $1:/$1 slavkodo/wakaama_pp_env 
$1

# remove container after success running (if your script says: wakaama_pp exists - run again ./wppstart.sh <name_exec_file>)                                                                                                                          
sudo docker rm -f wakaama_pp

