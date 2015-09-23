#!/bin/bash
HOST=flexelint.codac.local
REMOTE_DIR_CODE=MARTe2-dev
rsync -avz --delete -e ssh . $USER@$HOST:$REMOTE_DIR_CODE
<<<<<<< HEAD
ssh $USER@$HOST "/opt/FlexeLint/flint -i $REMOTE_DIR_CODE/Source/Core/L0Portability/ -i $REMOTE_DIR_CODE/Source/Core/L0TypeDev/ -i $REMOTE_DIR_CODE/Source/Core/L1Objects/  -i $REMOTE_DIR_CODE/Source/Core/L0Portability/OperatingSystem/Linux -i $REMOTE_DIR_CODE/Source/Core/L0Portability/OperatingSystem/Generic -i /opt/FlexeLint/supp/lnt -i $REMOTE_DIR_CODE/MakeDefaults/Lint/ -v marte_flint_eclipse.lnt | grep -v lint-linux.h"
=======
ssh $USER@$HOST "cd $REMOTE_DIR_CODE && /opt/FlexeLint/flint -i /opt/FlexeLint/supp/lnt -i ./MakeDefaults/Lint/ -v marte_flint_eclipse.lnt | grep -v lint-linux.h"
>>>>>>> refs/heads/#197_Any_Type_Conversion
