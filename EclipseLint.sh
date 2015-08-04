#!/bin/bash
HOST=flexelint.codac.local
REMOTE_DIR_CODE=MARTe2-dev
rsync -avz --delete -e ssh . $USER@$HOST:$REMOTE_DIR_CODE
ssh $USER@$HOST "/opt/FlexeLint/flint -i $REMOTE_DIR_CODE/Source/Core/L0Portability/ -i $REMOTE_DIR_CODE/Source/Core/L0Portability/OperatingSystem/Linux -i /opt/FlexeLint/supp/lnt -i $REMOTE_DIR_CODE/MakeDefaults/Lint/ -v marte_flint_eclipse.lnt | grep -v lint-linux.h"
