#!/bin/bash

cd "$(dirname "${BASH_SOURCE[0]}")"

#Generate unique topic names to be used on a training session, avoiding collisions between users

USERNAME=${1:-$USER}

TOPICS=()
# Generate 10 unique names
for i in $(seq 1 10); do
    TOPIC_I="mass-spring-${USERNAME}-${i}"
    TOPICS+=("$TOPIC_I")
done

#Update the configuration
TOPIC_0="${TOPICS[0]}"
TOPIC_1="${TOPICS[1]}"

cp mass-spring-1-template.xml mass-spring-1.xml
sed -i "s/TOPIC_NAME_0/${TOPIC_0}/g" mass-spring-1.xml
cp mass-spring-2-template.xml mass-spring-2.xml
sed -i "s/TOPIC_NAME_1/${TOPIC_1}/g" mass-spring-2.xml

