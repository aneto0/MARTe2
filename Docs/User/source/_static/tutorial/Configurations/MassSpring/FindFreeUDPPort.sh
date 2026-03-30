#!/bin/bash
#Generate unique UDP user ports to be used on a training session, avoiding collisions between users

BASE_MIN=40000
RANGE_SIZE=10000
NUM_PORTS=10

USER_HASH=$(echo -n "${USER}_$(id -u)" | cksum | awk '{print $1}')
OFFSET=$((USER_HASH % (RANGE_SIZE - NUM_PORTS)))
UDP_BASE_PORT=$((BASE_MIN + OFFSET))

PORTS=()

for ((i=0; i<NUM_PORTS; i++)); do
    PORT=$((UDP_BASE_PORT + i))

    while netstat -uln | grep -q ":$PORT"; do
        echo "Port $PORT in use, trying next one..."
        UDP_BASE_PORT=$((UDP_BASE_PORT + 1))
        PORT=$((UDP_BASE_PORT + i))
    done

    PORTS+=($PORT)
done

echo ${PORTS[@]}

