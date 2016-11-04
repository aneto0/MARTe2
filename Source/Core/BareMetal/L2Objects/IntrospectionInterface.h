/**
 * @file IntrospectionInterface.h
 * @brief Header file for class IntrospectionInterface
 * @date Oct 24, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class IntrospectionInterface
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L2OBJECTS_INTROSPECTIONINTERFACE_H_
#define L2OBJECTS_INTROSPECTIONINTERFACE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * Defines the API to access object internal information and meta information
 * supports different/expandable object meta-views
 * basic meta-views are:
 * 1) debugging view: to show the object internals for the purpose of debugging
 * 2) serialisation/deserialisation: structuredData able to construct an object
 * 3) user display: to provide the meaningful object content
 *
 */
class IntrospectionInterface{

public:

    ///
    struct  SerialiseContext{

        enum {
            /**
             * raw data display (pointers etc..)-no attempt to interpret complex fields
             * deserialisation updates one field
             * serialisation
             *       no recursion - always one level
             *       supports sub-addressing
             *       supports metadata as output
             *       output form (as CDB):
             *                     {
             *                         <var1> = {
             *                             class = <type or class name>
             *                             modifier = < sequence of modifiers >
             * [omitted for metadata]      address = < start pointer to memory> | value = < value as string>
             *                         }
             *                         <var2> = {
             *                             class = <type or class name>
             *                             modifier = < sequence of modifiers >
             * [omitted for metadata]      address = < start pointer to memory> | value = < value as string>
             *                         }
             *                         ....
             *                     }
             *       input form (as CDB):
             *
             *                     <var  dereferenced full address C style > = {
             * [optional]               interpret_class = <type name to be used in interpreting value>
             *                          value = < string of value>
             *                     }
             *
             */
            debugSerialisation,

            /**
             * deserialisation - not supported
             * serialisation
             *       display of meaningful data,
             *       actual output depends on userCode,
             *       recursion limited by depth,
             *       treats containers as members data seamlessly
             *       supports metadata as output
             *       does not support sub-addressing
             *       output form (as CDB):
             *                     {
             *                         <var1> = {
             *                             class = <class>
             *                             <var11> = {
             *                                 class = <type>
             * [omitted for metadata]          value = <value as string>
             *                             }
             *                             <var12> = {  // collapsed data due to recursion limit - just show metadata
             *                                 class = <class name>
             *                             }
             *                             Container = {
             *                                 <var13 = object name> = {
             *                                     class = <type>
             * [omitted for metadata]              value = <value as string>
             *                                 }
             *                                 <var14 = object name> = {  // collapsed data due to recursion limit - just show metadata
             *                                     class = <class name>
             *                                 }
             *                             }
             *                             ....
             *                       }
             *
             *
             */
            displaySerialisation,


            /**
             * serialisation / deserialisation
             *       display of object configuration parameters
             *       recursion limited by depth,
             *       treats containers recursively by calling deserialisation of each contained object
             *       supports metadata as output
             *       output form (as CDB):
             *                     {
             *                         <var1> = {
             *                             class = <class>
             *                             <var11> = {
             * [non-necessary for deserializ.] class = <type>
             * [omitted for metadata]          value = <value as string>
             *                             }
             *                             <var12> = {  // collapsed data due to recursion limit - just show metadata
             *                                 class = <class name>
             *                             }
             *                             Container = {
             *                                 <var13 = object name> = {
             *                                 " ser/des of contained object with same ser/des parameters but depth--"
             * [non-necessary for deserialisation] class = <type>
             * [omitted for metadata]              value = <value as string>
             *                                 }
             *                                 <var14 = object name> = {  // collapsed data due to recursion limit - just show metadata
             *                                     class = <class name>
             *                                 }
             *                             }
             *                     }
             *
             *
             * display of object config parameters, depending on navigateContainers will recurse through containers. Supports metadata as output
             *
             */
            objectSerialisation
        } mode:8;

        /// custom code to select different display views
        uint8 userCode:8;
    };

    ///
    struct SerialiseMode{
        enum {
            ///
            metaData,
            ///
            data
        } mode:8;

        /// how many levels of recursions
        uint8 depth:8;

        /// whether to navigate through containers
        bool navigateContainers:8;
    };

    /**
     *
     */
    ErrorManagement::ErrorType Serialise(SerialiseContext context,SerialiseMode mode,CCString address, StructuredDataI &out );

    /**
     *
     */
    ErrorManagement::ErrorType DeSerialise(SerialiseContext context,SerialiseMode mode,CCString address, const StructuredDataI &in );

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L2OBJECTS_INTROSPECTIONINTERFACE_H_ */
	
