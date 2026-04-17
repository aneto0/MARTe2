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
        MARTe::DataSourceI () {
    using namespace MARTe;
    ///AUTO-GENERATED: CTOR PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: END OF CTOR PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: CTOR SIGNALS. DO NOT EDIT!
    ///AUTO-GENERATED: END OF CTOR SIGNALS. DO NOT EDIT!

}

${class}::~${class}() {
    using namespace MARTe;
    ///AUTO-GENERATED: DTOR PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: END OF DTOR PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: DTOR SIGNALS. DO NOT EDIT!
    ///AUTO-GENERATED: END OF DTOR SIGNALS. DO NOT EDIT!
}

bool ${class}::Initialise(MARTe::StructuredDataI &data) {
    using namespace MARTe;
    bool ok = DataSourceI::Initialise(data);
    StructuredDataIHelper helper(data, this);

    ///AUTO-GENERATED: INITIALISE PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: END OF INITIALISE PARAMETERS. DO NOT EDIT!
    return ok;
}

bool ${class}::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    StructuredDataIHelper helper(data, this);

    ///AUTO-GENERATED: SETUP SIGNALS. DO NOT EDIT!
    ///AUTO-GENERATED: END OF SETUP SIGNALS. DO NOT EDIT!
    return ok;
}

bool ${class}::Synchronise() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool ${class}::AllocateMemory() {
    using namespace MARTe;
    bool ok = true;

    ///AUTO-GENERATED: ALLOCATE MEMORY SIGNALS. DO NOT EDIT!
    ///AUTO-GENERATED: END OF ALLOCATE MEMORY SIGNALS. DO NOT EDIT!
    return ok;
}

bool ${class}::GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void *&signalAddress) {
    using namespace MARTe;
    bool ok = true;
    ///AUTO-GENERATED: GET MEMORY SIGNALS. DO NOT EDIT!
    ///AUTO-GENERATED: END OF GET MEMORY SIGNALS. DO NOT EDIT!
    return ok;
}

const MARTe::char8 *${class}::GetBrokerName(MARTe::StructuredDataI &data, const MARTe::SignalDirection direction) {
    using namespace MARTe;
    const char8 *brokerName = "";
    ///AUTO-GENERATED: GET BROKER NAME. DO NOT EDIT!
    ///AUTO-GENERATED: END OF GET BROKER NAME. DO NOT EDIT!
    return brokerName;
}

bool ${class}::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

CLASS_REGISTER(${class}, "1.0")
}
