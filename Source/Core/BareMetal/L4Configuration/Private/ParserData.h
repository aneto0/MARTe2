/**
 * @file ParserData.h
 * @brief Header file for class ParserData
 * @date Apr 24, 2020
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef PRIVATE_PARSER_DATA_H_
#define PRIVATE_PARSER_DATA_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "Vector.h"
#include "CCString.h"
#include "RegularExpression.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

class  ParserData{
public:

   /*
   * The index of initial expected token identifier.
   */
   uint32 StartSymbol;

   /**
    * The end of the input.
    */
   uint32 EndOfFile;

   /**
    * The start index of the expected token identifiers array.
    */
   uint32 StartState;

   /**
    * The start index of the conflicts array.
    */
   uint32 StartConflict;

   /**
    * The end index of the conflicts array.
    */
   uint32 EndConflict;

   /**
    * The start index of the actions array.
    */
   uint32 StartAction;

   /**
    * The end index of the actions array.
    */
   uint32 EndAction;

   /**
    * The number of possible conflicts.
    */
   uint32 TotalConflicts;

   /**
    *
    */
   uint32 StackSize;

   /**
    * points to the production array
	* note Production[Production_row[ix]]
	* identifies a variable length structure as follow:
	*
 	* struct Production {
	*	uint32 length; //
	*	uint32
	* }
    */
   const uint32 *Production;

   /*
    * points to production_row array
    */
   const uint32 *Production_row;

   /*
    * points to parse array
    */
   const uint32 *Parse;

   /*
    * points to parse row array
    */
   const uint32 *Parse_row;

   /*
    * points to conflict array
    */
   const uint32 *Conflict;

   /*
    * points to conflict row array
    */
   const uint32 *Conflict_row;

   /*
    * vector  of strings
    */
   Vector<CCString> Nonterminal_name;

   /*
    * vector  of strings
    */
   Vector<CCString> Terminal_name;

   /*
    * vector  of strings
    */
   Vector<CCString> Action_name;

   /*
    * vector  of strings
    */
   Vector<CCString> Production_name;


   /**
    * defines the lexic and contains the id binding to the SLK grammar
    */
   const ZeroTerminatedArray<const RegularExpression::PatternInformation> ruleSet;

   /**
    *
    */
   ParserData(
		   	    uint32              startSymbolIn,
		   	   	uint32              endOfFileIn,
				uint32              startStateIn,
				uint32              startConflictIn,
				uint32              endConflictIn,
				uint32              startActionIn,
				uint32              endActionIn,
				uint32              totalConflictsIn,
				uint32              stackSizeIn,
				const uint32 *      ProductionIn,
				const uint32 *		Production_rowIn,
				const uint32 *		ParseIn,
				const uint32 *		Parse_rowIn,
				const uint32 *		ConflictIn,
				const uint32 *		Conflict_rowIn,
				const char **		Nonterminal_nameIn,
				uint32				Nonterminal_name_size,
				const char **		Terminal_nameIn,
				uint32				Terminal_name_size,
				const char **		Action_nameIn,
				uint32				Action_name_size,
				const char **		Production_nameIn,
				uint32				Production_name_size,
				const ZeroTerminatedArray<const RegularExpression::PatternInformation> ruleSetIn

   ):
	   StartSymbol               (startSymbolIn              ),
	   EndOfFile                 (endOfFileIn                ),
	   StartState                (startStateIn               ),
	   StartConflict             (startConflictIn            ),
	   EndConflict               (endConflictIn              ),
	   StartAction               (startActionIn              ),
	   EndAction                 (endActionIn                ),
	   TotalConflicts            (totalConflictsIn           ),
	   StackSize                 (stackSizeIn                ),
	   Production                (ProductionIn               ),
	   Production_row            (Production_rowIn           ),
	   Parse                     (ParseIn                    ),
	   Parse_row                 (Parse_rowIn                ),
	   Conflict                  (ConflictIn                 ),
	   Conflict_row              (Conflict_rowIn             ),
       Nonterminal_name(reinterpret_cast<CCString *>(&Nonterminal_nameIn[0]),Nonterminal_name_size),
   	   Terminal_name   (reinterpret_cast<CCString *>(&Terminal_nameIn[0])	,Terminal_name_size),
	   Action_name     (reinterpret_cast<CCString *>(&Action_nameIn[0])		,Action_name_size),
	   Production_name (reinterpret_cast<CCString *>(&Production_nameIn[0])	,Production_name_size),
	   ruleSet(ruleSetIn)

   {
   }
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe


#endif /* PARSERDATA_H_ */

