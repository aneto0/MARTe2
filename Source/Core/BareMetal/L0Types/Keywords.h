/**
 * @file Keywords.h
 * @brief Header file for class Keywords
 * @date 05/08/2016
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

 * @details This header file contains the declaration of the class Keywords
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef KEYWORDS_H_
#define KEYWORDS_H_
/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"

namespace MARTe {
namespace Keyword {

__attribute((unused))
static const char8 * FUNCTIONS = "Functions";
__attribute((unused))
static const char8 * DATA = "Data";
__attribute((unused))
static const char8 * STATES = "States";
__attribute((unused))
static const char8 * THREADS = "Threads";
__attribute((unused))
static const char8 * CLASS = "Class";
__attribute((unused))
static const char8 * QUALIFIED_NAME = "QualifiedName";
__attribute((unused))
static const char8 * TYPE = "Type";
__attribute((unused))
static const char8 * FULL_TYPE = "FullType";
__attribute((unused))
static const char8 * NUMBER_OF_ELEMENTS = "NumberOfElements";
__attribute((unused))
static const char8 * NUMBER_OF_DIMENSIONS = "NumberOfDimensions";
__attribute((unused))
static const char8 * SIGNALS = "Signals";
__attribute((unused))
static const char8 * INPUT_SIGNALS = "InputSignals";
__attribute((unused))
static const char8 * OUTPUT_SIGNALS = "OutputSignals";
__attribute((unused))
static const char8 * DEFAULT = "Default";
__attribute((unused))
static const char8 * ALIAS = "Alias";
__attribute((unused))
static const char8 * SAMPLES = "Samples";
__attribute((unused))
static const char8 * FREQUENCY = "Frequency";
__attribute((unused))
static const char8 * RANGES = "Ranges";
__attribute((unused))
static const char8 * DATA_SOURCE = "DataSource";


}
}

#endif /* KEYWORDS_H_ */
