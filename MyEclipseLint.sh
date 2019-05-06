#!/bin/bash
HOST=192.168.130.72
#flexelint.codac.local
REMOTE_DIR_CODE=/home/gferro/MARTe2-dev
rsync -avz --delete -e ssh . root@$HOST:$REMOTE_DIR_CODE
ssh root@$HOST "cd $REMOTE_DIR_CODE && /opt/FlexeLint/flint -i /opt/FlexeLint/supp/lnt -i ./MakeDefaults/Lint/ -v marte_flint_eclipse.lnt | grep -v lint-linux.h"
