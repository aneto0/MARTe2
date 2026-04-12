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
        DataSourceI() {
    using namespace MARTe;
}

${class}::~${class}() {
    using namespace MARTe;
}

bool ${class}::Initialise(MARTe::StructuredDataI &data) {
    using namespace MARTe;
    bool ok = DataSourceI::Initialise(data);
    StructuredDataIHelper helper(data, this);

    return ok;
}

bool ${class}::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    StructuredDataIHelper helper(data, this);
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
    return ok;
}

bool ${class}::GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void *&signalAddress) {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

const MARTe::char8 *${class}::GetBrokerName(MARTe::StructuredDataI &data, const MARTe::SignalDirection direction) {
    using namespace MARTe;
    const char8 *brokerName = "";
    return brokerName;
}

bool ${class}::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

CLASS_REGISTER(${class}, "1.0")
}

