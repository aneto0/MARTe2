/**
 * @file XMLParser.h
 * @brief Header file for class XMLParser
 * @date 09/12/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class XMLParser
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "ParserI.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*lint -save -e754 -e750 -e753*/
namespace MARTe {

class XMLParser: public ParserI {
public:

    XMLParser(StreamI &stream,
              ConfigurationDatabase &databaseIn,
              BufferedStreamI * const err = static_cast<BufferedStreamI*>(NULL));

    virtual ~XMLParser();

protected:

    virtual uint32 &GetProduction(const uint32 index) const;

    virtual uint32 GetProductionRow(const uint32 index) const;

    virtual uint32 GetParse(const uint32 index) const;

    virtual uint32 GetParseRow(const uint32 index) const;

    virtual uint32 GetConflict(const uint32 index) const;

    virtual uint32 GetConflictRow(const uint32 index) const;

    virtual uint32 GetConstant(const uint32 index) const;

    virtual const char8 *GetSymbolName(const uint32 symbol) const;

    virtual void Execute(const uint32 number);


private:
    // functions void f()
    void (XMLParser::*Action[10])(void);

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* XMLPARSER_H_ */

