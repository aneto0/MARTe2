/**
 * @file ObjectBrowser.h
 * @brief Header file for class ObjectBrowser
 * @date 29/10/2018
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class ObjectBrowser
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4CONFIGURATION_OBJECTBROWSER_H_
#define L4CONFIGURATION_OBJECTBROWSER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataExportI.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief TODO
 */
class ObjectBrowser: public ReferenceContainer, public DataExportI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief TODO
     */
ObjectBrowser    ();

    /**
     * @brief TODO
     */
    virtual ~ObjectBrowser();

    /**
     * @brief TODO
     */
    virtual void Purge(ReferenceContainer &purgeList);

    /**
     * @brief TODO
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief TODO
     */
    virtual bool GetAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol);

    /**
     * @brief TODO
     */
    virtual bool GetAsText(StreamI &stream, ProtocolI &protocol);


private:
    /**
     * @brief TODO
     */
    bool GetTargetAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol, Reference &target);

    /**
     * @brief TODO
     */
    Reference FindTarget(ProtocolI &protocol);

    /**
     * TODO
     */
    Reference root;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4CONFIGURATION_OBJECTBROWSER_H_ */

