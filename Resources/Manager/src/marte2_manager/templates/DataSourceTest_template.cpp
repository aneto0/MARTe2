${header_text}

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "${class}.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace ${namespace} {

${class}::${class}() {
    using namespace MARTe;
}

${class}::~${class}() {
    using namespace MARTe;
}

bool ${class}::TestInitialise() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool ${class}::TestSetConfiguredDatabase() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool ${class}::TestSynchronise() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool ${class}::TestAllocateMemory() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool ${class}::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool ${class}::TestGetBrokerName() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool ${class}::TestPrepareNextState() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

}
