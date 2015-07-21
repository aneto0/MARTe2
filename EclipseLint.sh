#!/bin/bash
HOST=flexelint.codac.local
REMOTE_DIR_CODE=MARTe2-dev
REMOTE_DIR_DOC=MARTe2-doc
LOCAL_DIR_DOC=../MARTe2-doc/.
rsync -avz --delete -e ssh . iherrero@$HOST:$REMOTE_DIR_CODE
rsync -avz --delete -e ssh $LOCAL_DIR_DOC iherrero@$HOST:$REMOTE_DIR_DOC
ssh iherrero@$HOST "/opt/FlexeLint/flint -i $REMOTE_DIR_CODE/Source/Core/L0Portability/ -i $REMOTE_DIR_CODE/Source/Core/L0Portability/OperatingSystem/Linux -i /opt/FlexeLint/supp/lnt -i $REMOTE_DIR_DOC/Assets/Configuration/ -v marte_flint_eclipse.lnt | grep -v lint-linux.h"
