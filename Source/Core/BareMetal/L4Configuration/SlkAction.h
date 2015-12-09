/**
 * @file SlkAction.h
 * @brief Header file for class SlkAction
 * @date 04/12/2015
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

 * @details This header file contains the declaration of the class SlkAction
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SLKACTION_H_
#define SLKACTION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "SlkError.h"
#include "SlkToken.h"
#include "StaticListHolder.h"
#include "ConfigurationDatabase.h"
#include "StreamString.h"
#include "AnyTypeCreator.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class SlkAction {

#include "SlkTable.h"

    SlkAction(StreamI &stream,
              ConfigurationDatabase &databaseIn,
              BufferedStreamI * const err = NULL,
              ParserGrammar grammarIn = StandardGrammar);

              ~SlkAction();

              bool Parse();

              ParserGrammar GetGrammar() const;

              void predict(uint16 a);

          private:
              /*
               void GetTypeCast();

               void BlockEnd();

               void CreateNode();

               void AddLeaf();

               void GetNodeName();

               void AddScalar();

               void EndRow();
               */
              void CreateClassLeaf();

              StreamString typeName;

              StreamString nodeName;

              ConfigurationDatabase *database;

              SlkError parseError;

              SlkToken token;

              uint32 numberOfColumns;

              uint32 firstNumberOfColumns;

              uint32 numberOfRows;

              AnyTypeCreator memory;

              BufferedStreamI *errorStream;

              uint32 tokenType;

              uint32 numberOfDimensions;

              ParserGrammar grammar;
          };

          /*---------------------------------------------------------------------------*/
          /*                        Inline method definitions                          */
          /*---------------------------------------------------------------------------*/

#endif /* SLKACTION_H_ */

