${header_text}

#ifndef ${header_guard}
#define ${header_guard}

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "${src_class}.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace ${namespace} {

/**
 * @brief TODO.
 *
 * @details TODO.
 */

class ${class} {
public:
    ${class}();

    virtual ~${class}();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Setup method.
     */
    bool TestSetup();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ${header_guard} */

