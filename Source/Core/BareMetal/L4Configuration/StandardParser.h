/**
 * @file StandardParser.h
 * @brief Header file for class StandardParser
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

 * @details This header file contains the declaration of the class StandardParser
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STANDARDPARSER_H_
#define STANDARDPARSER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ParserI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

class StandardParser: public ParserI {

public:

    StandardParser(StreamI &stream,
                   ConfigurationDatabase &databaseIn,
                   BufferedStreamI * const err=NULL);

    virtual ~StandardParser();

protected:

    virtual uint32 &GetProduction(const uint32 index)const ;

    virtual uint32 GetProductionRow(const uint32 index)const ;

    virtual uint32 GetParse(const uint32 index)const ;

    virtual uint32 GetParseRow(const uint32 index)const ;

    virtual uint32 GetConflict(const uint32 index)const ;

    virtual uint32 GetConflictRow(const uint32 index)const ;

    virtual uint32 GetConditionalProduction(const uint32 symbol)const ;

    virtual uint32 GetPredictedEntry(const uint32 productionNumber,
                                     const uint32 tokenId,
                                     const uint32 level,
                                     const uint32 x)const ;

    virtual uint32 GetConstant(const uint32 index)const ;

    virtual const char8 *GetSymbolName(const uint32 symbol)const ;

    virtual void Execute(uint32 number);


private:
    // functions void f()
    void (StandardParser::*Action[10])(void);

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STANDARDPARSER_H_ */

