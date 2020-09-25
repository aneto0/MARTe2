#!/bin/bash
#Execute the QAHelper with the most common parameters for the MARTe core.
#Run Resources/QA/QAHelper.py -h to see all available options.

#Pass directly any extra args to the command to be executed. To skip the actual execution of the tests use -le -ge.
EXTRA_ARGS=""

#Consume input arguments
while [[ $# -ge 1 ]]
do
    EXTRA_ARGS="$EXTRA_ARGS ""$1"
shift
done

GTEST_FILTER="Bare* Scheduler* FileS*"
TEST_EXCEPTIONS="CompilerTypes.h BufferedStreamGenerator.h IteratorT.h GeneralDefinitions.h StaticListHolder.h GrammarInfo.h ConfigurationParserI.h ParserI.h VariableInformation.h"

USERNAME=`whoami`
USER_RECORD="$(getent passwd $USERNAME)"
USER_GECOS_FIELD="$(echo "$USER_RECORD" | cut -d ':' -f 5)"
USER_FULL_NAME="$(echo "$USER_GECOS_FIELD" | cut -d ',' -f 1)"

CMD="Resources/QA/QAHelper.py"
$CMD -lf $GTEST_FILTER -gf $GTEST_FILTER -ru "$USER_FULL_NAME" -fe $TEST_EXCEPTIONS $EXTRA_ARGS

