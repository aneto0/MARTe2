#!/usr/bin/env bash
set -euo pipefail

# Name of the env variable that should contain the destination path
: "${MARTE_TRAINING_PARENT_DIR:?Error: MARTE_TRAINING_PARENT_DIR environment variable is not set}"


DEST_DIR="$MARTE_TRAINING_PARENT_DIR/MARTe2-training-proj"
# Check destination does NOT exist
if [[ -e "$DEST_DIR" ]]; then
    echo "Error: Destination '$DEST_DIR' already exists"
    exit 1
fi

# Copy directory
rsync -av --delete --exclude='.git' ./ $DEST_DIR/

echo "Copied '.' -> '$DEST_DIR'"
