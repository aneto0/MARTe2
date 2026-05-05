#!/bin/bash

cd "$(dirname "${BASH_SOURCE[0]}")"

#Generate unique topic names to be used on a training session, avoiding collisions between users

USERNAME=${1:-$USER}

TOPICS=()
# Generate 10 unique names
for i in $(seq 0 9); do
    TOPIC_I="mass-spring-${USERNAME}-${i}"
    TOPICS+=("$TOPIC_I")
done

#Update the configuration
TOPIC_0="${TOPICS[0]}"
TOPIC_1="${TOPICS[1]}"
TOPIC_2="${TOPICS[2]}"
TOPIC_3="${TOPICS[3]}"

cp mass-spring-0-template.xml mass-spring-0.xml
sed -i "s/TOPIC_NAME/${TOPIC_0}/g" mass-spring-0.xml
cp mass-spring-1-template.xml mass-spring-1.xml
sed -i "s/TOPIC_NAME/${TOPIC_1}/g" mass-spring-1.xml
cp mass-spring-2-template.xml mass-spring-2.xml
sed -i "s/TOPIC_NAME/${TOPIC_2}/g" mass-spring-2.xml
cp mass-spring-3-template.xml mass-spring-3.xml
sed -i "s/TOPIC_NAME/${TOPIC_3}/g" mass-spring-3.xml

