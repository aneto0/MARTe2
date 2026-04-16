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
    ///AUTO-GENERATED: CTOR PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: CTOR END OF PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: CTOR SIGNALS. DO NOT EDIT!
    ///AUTO-GENERATED: CTOR END OF SIGNALS. DO NOT EDIT!
}

${class}::~${class}() {
    using namespace MARTe;
    ///AUTO-GENERATED: DTOR PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: DTOR END OF PARAMETERS. DO NOT EDIT!
}

bool ${class}::Initialise(MARTe::StructuredDataI &data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    StructuredDataIHelper helper(data, this);

    ///AUTO-GENERATED: INITIALISE PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: INITIALISE END OF PARAMETERS. DO NOT EDIT!

    return ok;
}

bool ${class}::Setup() {
    using namespace MARTe;
    bool ok = true;
    ///AUTO-GENERATED: SETUP SIGNALS. DO NOT EDIT!
    ///AUTO-GENERATED: SETUP END OF SIGNALS. DO NOT EDIT!
    return ok;
}

bool ${class}::Execute() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

CLASS_REGISTER(${class}, "1.0")
} 

