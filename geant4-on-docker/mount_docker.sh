#!/usr/bin/bash

# pull the image from Zoya's hub if not exist
if [[ "$(docker images -q zoyavallari/g4pybuild:latest 2> /dev/null)" == "" ]]; then
    docker pull zoyavallari/g4pybuild
fi

# swith to the home directory
cd ~

# mount the current folder as /root and enter the shell environment
docker run -v ${PWD}:/root -it zoyavallari/g4pybuild
