${header_text}

#ifndef ${header_guard}
#define ${header_guard}

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace ${namespace} {

/**
 * @brief TODO.
 *
 * @details TODO.
 */

///AUTO-GENERATED: CLASS DOC. DO NOT EDIT!
///AUTO-GENERATED: END OF CLASS DOC. DO NOT EDIT!

class ${class} : public MARTe::ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. 
     * @details TODO.
     */
    ${class} ();

    /**
     * @brief Destructor. 
     * @details TODO.
     */
    virtual ~${class} ();

    /**
     * @brief Initialises the components. TODO
     * @details TODO. 
     * @param[in] data The component configuration parameters. TODO
     * @return TODO.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

private:
    ///AUTO-GENERATED: PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: END OF PARAMETERS. DO NOT EDIT!

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* ${header_guard}*/
