${header_text}

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "${class}.h"
#include "StructuredDataIHelper.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace ${namespace} {

${class}::${class}() :
        GAM() {
    using namespace MARTe;
}

${class}::~${class}() {
    using namespace MARTe;
}

bool ${class}::Initialise(MARTe::StructuredDataI &data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    StructuredDataIHelper helper(data, this);

    return ok;
}

bool ${class}::Setup() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool ${class}::Execute() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

CLASS_REGISTER(${class}, "1.0")
} 

