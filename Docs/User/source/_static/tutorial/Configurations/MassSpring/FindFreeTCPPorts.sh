#!/bin/bash
#Generate unique TCP user ports to be used on a training session, avoiding collisions between users

BASE_MIN=4840
RANGE_SIZE=1000
NUM_PORTS=2

USER_HASH=$(echo -n "${USER}_$(id -u)" | cksum | awk '{print $1}')
OFFSET=$((USER_HASH % (RANGE_SIZE - NUM_PORTS)))
TCP_BASE_PORT=$((BASE_MIN + OFFSET))

PORTS=()

for ((i=0; i<NUM_PORTS; i++)); do
    PORT=$((TCP_BASE_PORT + i))

    while netstat -tln | grep -q ":$PORT"; do
        TCP_BASE_PORT=$((TCP_BASE_PORT + 1))
        PORT=$((TCP_BASE_PORT + i))
    done

    PORTS+=($PORT)
done

echo ${PORTS[@]}

