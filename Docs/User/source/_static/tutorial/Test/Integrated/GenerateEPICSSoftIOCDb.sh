#!/bin/bash

cd "$(dirname "${BASH_SOURCE[0]}")"

#Generate unique topic names to be used on a training session, avoiding collisions between users

USERNAME=${1:-$USER}
USERNAME_CAPS="${USERNAME^^}"

cp MassSpring-Template.db MassSpring-1.db 
sed -i "s/USERNAME/${USERNAME_CAPS}/g" MassSpring-1.db 

