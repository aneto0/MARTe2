#!/bin/bash
HOST=flexelint.codac.local
REMOTE_DIR_CODE=MARTe2-dev
USER=codac
rsync -avz  --exclude 'Lib' --exclude 'Test' --exclude '.git' --exclude 'Build' --exclude 'cov' --delete -e ssh . $USER@$HOST:$REMOTE_DIR_CODE
ssh $USER@$HOST "cd $REMOTE_DIR_CODE && /opt/FlexeLint/flint -i /opt/FlexeLint/supp/lnt -i ./MakeDefaults/Lint/ -v marte_flint_eclipse.lnt | grep -v lint-linux.h"
