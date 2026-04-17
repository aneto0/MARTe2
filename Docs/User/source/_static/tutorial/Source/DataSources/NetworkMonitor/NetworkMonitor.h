/**                                                                              
 * @file NetworkMonitor.h                                                                 
 * @brief Header file for class NetworkMonitor                                         
 * @date 17/04/2026                                                                 
 * @author cabrian                                                             
 *                                                                               
 * @copyright Copyright 2026 F4E | European Joint Undertaking for ITER and       
 * the Development of Fusion Energy ('Fusion for Energy').                       
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved       
 * by the European Commission - subsequent versions of the EUPL (the 'Licence')  
 * You may not use this work except in compliance with the Licence.              
 * You may obtain a copy of the Licence at: https://eupl.eu/                     
 *                                                                               
 * @warning Unless required by applicable law or agreed to in writing,           
 * software distributed under the Licence is distributed on an  'AS IS'          
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express           
 * or implied. See the Licence permissions and limitations under the Licence.    

 * @details This header file contains the declaration of the class NetworkMonitor      
 * with all of its public, protected and private members. It may also include    
 * definitions for inline methods which need to be visible to the compiler.      
 */                                                                              

#ifndef _TUTORIAL_NETWORKMONITOR_H_
#define _TUTORIAL_NETWORKMONITOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "File.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace Tutorial {

/**
 * @brief TODO.
 *
 * @details TODO.
 */

///AUTO-GENERATED: CLASS DOC. DO NOT EDIT!
/**
 * @details
 * +NetworkMonitorInstance = {
 *     Class = NetworkMonitor
 *     NICName = "" //Required. Name of the network interface card to monitor
 *     Signals = {
 *         RXPackets = {//Number of RX packets
 *             Type = uint64
 *         }
 *         TXPackets = {//Number of TX packets
 *             Type = uint64
 *         }
 *         RXDropped = {//Number of dropped RX packets
 *             Type = uint64
 *         }
 *         TXDropped = {//Number of dropped TX packets
 *             Type = uint64
 *         }
 *     }
 * }
 */
///AUTO-GENERATED: END OF CLASS DOC. DO NOT EDIT!

class NetworkMonitor : public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. 
     * @details TODO.
     */
    NetworkMonitor ();

    /**
     * @brief Destructor. 
     * @details TODO.
     */
    virtual ~NetworkMonitor ();

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

    /**
     * Name of the network interface card to monitor
     */
    MARTe::StreamString nicName;
    ///AUTO-GENERATED: END OF PARAMETERS. DO NOT EDIT!

    ///AUTO-GENERATED: SIGNALS. DO NOT EDIT!

    /**
     * Number of RX packets
     */
    MARTe::uint64 rxPackets;

    /**
     * Number of TX packets
     */
    MARTe::uint64 txPackets;

    /**
     * Number of dropped RX packets
     */
    MARTe::uint64 rxDropped;

    /**
     * Number of dropped TX packets
     */
    MARTe::uint64 txDropped;
    ///AUTO-GENERATED: END OF SIGNALS. DO NOT EDIT!

    /**
     * TXPackets file
     */
    MARTe::File txPacketsFile;

    /**
     * RXPackets file
     */
    MARTe::File rxPacketsFile;

    /**
     * TXDropped file
     */
    MARTe::File txDroppedFile;

    /**
     * RXDropped file
     */
    MARTe::File rxDroppedFile;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* _TUTORIAL_NETWORKMONITOR_H_*/
