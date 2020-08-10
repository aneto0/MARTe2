/**
 * @file ObjectDataModel.h
 * @brief Header file for class ObjectDataModel
 * @date Nov 15, 2016
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

 * @details This header file contains the declaration of the class ObjectDataModel
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BAREMETAL_L2OBJECTS_OBJECTDATAMODEL_H_
#define BAREMETAL_L2OBJECTS_OBJECTDATAMODEL_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/



/**
 *
 * ObjectModel = {
 *
 *     /// members of the object
 *     /// name of the member = type of the member
 *     /// types must be introspectable
 *     ObjectProperties = {
 *         INPUT      = InputType
 *         OUTPUT     = OutputType
 *         PARAMS     = ParamType
 *         CONFIG     = ConfigType
 *         RTSTATUS   = RtStatusType
 *         PERSISTENT = PersistentType
 *         DISPLAY    = DisplayType
 *         STATUS     = StatusType
 *     }
 *     /// Private Methods - interface will be produced
 *     /// Implementation for the user to complete
 *     /// Methods will be callable via the interfaces
 *     /// IN  = const type &
 *     /// OUT = type &
 *     ObjectMethods = {
 *         ELABORATE = {  //--> ELABORATE (const STATUS &p1, const INPUT &in2, const PARAMS &in3, OUTPUT &out1, RTSTATUS &io1);
 *             IN= {STATUS,INPUT,PARAMS,RTSTATUS}
 *             OUT = {OUTPUT,RTSTATUS}
 *         }
 *         CONFIGURE = {
 *             IN= {CONFIG,PARAM,STATUS,RTSTATUS,PERSISTENT}
 *             OUT= {PARAM,STATUS,RTSTATUS}
 *         }
 *         SAVE = {
 *             IN= {PARAM,STATUS,PERSISTENT}
 *             OUT= {PERSISTENT}
 *         }
 *         CLONE = {
 *             IN= {PARAM}
 *             OUT={CONFIG}
 *         }
 *         SERIALISE = {
 *             IN= {PARAM,STATUS,PERSISTENT}
 *             OUT={CONFIG}
 *         }
 *     }
 *     /// external methods - name must be different from ObjectMethods
 *     /// allows affecting one of the properties using the interface
 *     /// STRUCTUREDDATAI --> function with StructuredDataI registered
 *     /// DIRECT -->
 *     /// after/before changing parameters launches trigger(s) POST/PRE
 *     ObjectInterfaces = {
 *         CONFIG={
 *             DATA = CONFIG
 *             IN=STRUCTUREDDATAI
 *             POST=CONFIGURE
 *         }
 *         DEBUG = {
 *             DATA = PARAM
 *             OUT=STRUCTUREDDATAI
 *         }
 *         CLONE = {
 *             DATA = CONFIG
 *             OUT=STRUCTUREDDATAI
 *             PRE=CLONE
 *         }
 *
 *     }
 *
 * }
 *
 *
 *
 */



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BAREMETAL_L2OBJECTS_OBJECTDATAMODEL_H_ */
	
