${header_text}

#ifndef ${header_guard}
#define ${header_guard}

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"

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

class ${class} : public MARTe::DataSourceI {
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
     * @brief Initialises the DataSource. TODO
     * @details TODO. 
     * @param[in] data The DataSource configuration parameters. TODO
     * @return TODO.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Sets the configured database for the DataSource. TODO
     * @details This method is called by the RealTimeApplicationConfigurationBuilder once all the RealTimeApplication information has been validated and negotiated. 
     * @param[in] data The configuration data.
     * @return TODO.
     */
    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

    /**
     * @brief Synchronises the DataSource. TODO
     * @details TODO. 
     * @return TODO.
     */
    virtual bool Synchronise();

    /**
     * @brief Allocate the memory for this DataSourceI.
     * @details TODO.  
     * @return true if the memory can be successfully allocated.
     */
    virtual bool AllocateMemory();

    /**
     * @brief Gets the memory address for the signal at index \a signalIdx.
     * @details TODO.   
     * @param[in] signalIdx the index of the signal.
     * @param[in] bufferIdx the index of the state buffer (see GetNumberOfStatefulMemoryBuffers()). Not to be confused with the buffer index given by GetNumberOfMemoryBuffers (which is handled with the Offset functions below).
     * @param[out] signalAddress a pointer to the memory address of this signal for this \a bufferIdx.
     * @return true if the signalIdx and the bufferIdx exist and the memory address can be retrieved for this signal.
     * @pre
     *   signalIdx < GetNumberOfSignals() &&
     *   bufferIdx < GetNumberOfMemoryBuffers()
     */
    virtual bool GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void *&signalAddress);

    /**
     * @brief Gets the name of the broker for the signal information available in the input \a data.
     * @details TODO.
     * @param[in] data the information about the signal. The structure is:
     * <pre>
     *   QualifiedName = "Name of the signal"
     *   NumberOfDimensions = N
     *   NumberOfElements = N
     *   Samples = N
     *   Frequency = N
     *   Trigger = N
     * </pre>
     * @param[in] direction the signal direction.
     * @return the name of the BrokerI class that will handle the copy of this signal from the DataSourceI memory to the GAM memory.
     */
    virtual const MARTe::char8 *GetBrokerName(MARTe::StructuredDataI &data, const MARTe::SignalDirection direction);

    /**
     * @brief Function called every time there is state change request.
     * @details TODO.
     * @param[in] currentStateName the name of the current state being executed.
     * @param[in] nextStateName the name of the next state to be executed.
     * @return true if the state change is accepted by this component.
     */
    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName);

private:

    ///AUTO-GENERATED: PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: END OF PARAMETERS. DO NOT EDIT!

    ///AUTO-GENERATED: SIGNALS. DO NOT EDIT!
    ///AUTO-GENERATED: END OF SIGNALS. DO NOT EDIT!
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* ${header_guard}*/
