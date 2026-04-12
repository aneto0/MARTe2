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

bool ${class}::TestSetup() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool ${class}::TestExecute() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}
}
