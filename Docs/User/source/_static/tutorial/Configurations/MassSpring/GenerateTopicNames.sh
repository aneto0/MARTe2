#!/bin/bash
#Generate unique topic names to be used on a training session, avoiding collisions between users

USERNAME=${1:-$USER}

TOPICS=()
# Generate 10 unique names
for i in $(seq 0 9); do
    TOPIC_I="mass-spring-${USERNAME}-${i}"
    TOPICS+=($TOPIC_I)
done

echo ${TOPICS[@]}

