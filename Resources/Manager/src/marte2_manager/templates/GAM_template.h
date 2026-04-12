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

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace ${namespace} {

/**
 * @brief TODO.
 *
 * @details TODO.
 */

class ${class} : public MARTe::GAM {
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
     * @brief Initialises the GAM. TODO
     * @details TODO. 
     * @param[in] data The GAM configuration parameters. TODO
     * @return TODO.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Perform post-initialise configurations. TODO 
     * @return TODO
     */
    virtual bool Setup();

    /**
     * @brief Executes the GAM logic. TODO
     * @return TODO.
     */
     virtual bool Execute();

private:

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* ${header_guard}*/
