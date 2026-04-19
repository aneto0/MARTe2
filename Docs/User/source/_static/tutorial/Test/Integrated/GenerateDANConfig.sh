#!/bin/bash

cd "$(dirname "${BASH_SOURCE[0]}")"

#Generate unique stream names to be used on a training session, avoiding collisions between users

USERNAME=${1:-$USER}
USERNAME_C="${USERNAME^}"

cp dan_mass_spring_demo_template.xml  dan_mass_spring_demo_0.xml
sed -i "s/USERNAME/${USERNAME_C}/g" dan_mass_spring_demo_0.xml

