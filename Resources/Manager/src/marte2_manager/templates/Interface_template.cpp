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
        ReferenceContainer() {
    using namespace MARTe;
}

${class}::~${class}() {
    using namespace MARTe;
}

bool ${class}::Initialise(MARTe::StructuredDataI &data) {
    using namespace MARTe;
    bool ok = ReferenceContainer::Initialise(data);
    StructuredDataIHelper helper(data, this);

    return ok;
}

CLASS_REGISTER(${class}, "1.0")
} 
