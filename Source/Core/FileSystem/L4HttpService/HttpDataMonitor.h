/**
 * @file HttpDataMonitor.h
 * @brief Header file for class HttpDataMonitor
 * @date 01/08/2025
 * @author Giuseppe Ferro
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class HttpDataMonitor
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4HTTPSERVICE_HTTPDATAMONITOR_H_
#define L4HTTPSERVICE_HTTPDATAMONITOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpDataExportI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Exports data from multiple components in order to be collected and shown by HTTP client.
 *
 * @details The user can define group of data sets. The objective is that each of this data sets is plotted from the
 * HTTP client. The default client implementation of HttpDataMonitor client employs the plotly.js library.
 *
 * @details Follows an example of configuration
 * <pre>
 *    +Monitor = {
 *        Class = HttpDataMonitor
 *        Plot1 = {
 *            Components = {"Test.Functions.GAM1"}
 *            Signals = {"OutputSignals.Counter"} //single signal
 *        }
 *        Plot2 = {
 *            Components = {"Test.Functions.Constant"}
 *            Signals = {"OutputSignals"} //all the signals in OutputSignals node
 *        }
 *        Plot3 = {
 *            Components = {"Test.Data.DDB1", "Test.Data.DDB1"}
 *            Signals = {"Signals.Value0", "Signals.Value1"} //all the signals in OutputSignals node
 *        }
 *    }
 * </pre>
 *
 * Assuming that the Component in Plot2 (in this case a GAM) contains "Constant1" and "Constant2" in "OutputSignals".
 * The exported data will be the following;
 * <pre>
 *    Plots = {
 *        Name = Monitor
 *        Class = HttpDataMonitor
 *        Plot1 = {
 *            Signals = {
 *                Test@Functions@GAM1@OutputSignals@Counter = 12
 *            }
 *            __Timestamp = 32343 //milliseconds
 *        }
 *        Plot2 = {
 *            Signals = {
 *                Test@Functions@Constant@OutputSignals@Constant1 = 0
 *                Test@Functions@Constant@OutputSignals@Constant2 = 10
 *            }
 *            __Timestamp = 32356 //milliseconds
 *        }
 *        Plot3 = {
 *            Signals = {
 *                Test@Data@DDB1@Signals@Value0 = 33
 *                Test@Data@DDB1@Signals@Value1 = 34
 *            }
 *            __Timestamp = 32479 //milliseconds
 *        }
 *    }
 * </pre>
 */
class DLL_API HttpDataMonitor: public ReferenceContainer, public HttpDataExportI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. Initialises the attributes.
     */
    HttpDataMonitor();

    /**
     * @brief Destructor. Destroys the attributes.
     */
    virtual ~HttpDataMonitor();

    /**
     * @brief Configures the component.
     * @param[in] data. The configuration data.
     * @details The user must define a list of data set nodes each one containing the following arrays (of the same size).
     *   - Components: the list of components paths where the signals come from
     *   - Signals: the list of signals (or signal sets), one per component (component can be repeated)
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Exports the data to be shown by the HTTP client.
     * @param[out] data. The output data.
     * @param[in,out] protocol. The data received by the client and at the same time to be sent to the client.
     */
    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    /**
     * @brief NOOP
     */
    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

private:

    /**
     * A list of components array per data set.
     */
    StreamString **componentPath;

    /**
     * A list of components references per data set
     */
    Reference **component;

    /**
     * The number of data sets
     */
    uint32 numberOfPlots;

    /**
     * The number of signals per data set
     */
    uint32 *numberOfSignals;

    /**
     * The data set name
     */
    StreamString *plotName;

    /**
     * A list of signal names per data set
     */
    StreamString **signalName;

    /**
     * A list of full signal paths per data set
     */
    StreamString **fullSignalName;

    /**
     * Flag if the component has been configured
     */
    bool configured;

    /**
     * Store the initial timestamp in milliseconds
     */
    uint64 initialMillis;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4HTTPSERVICE_HTTPDATAEXPORTI_H_ */

