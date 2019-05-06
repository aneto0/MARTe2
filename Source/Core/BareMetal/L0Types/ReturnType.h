/**
 * @file ReturnType.h
 * @brief Header file for class ReturnType
 * @date Apr 20, 2016
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

 * @details This header file contains the declaration of the class ReturnType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L0TYPES_RETURNTYPE_H_
#define L0TYPES_RETURNTYPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


namespace MARTe {

/**
 * TODO
 * */




















class DLL_API ReturnType{
public:
    /**
     * TODO
     * if allOk is false then the error.functionReturn is set to false
     * all other errors are always cleared (set to 1)
     * */
    ReturnType(bool allOk=false);

    /**
     * TODO
     * is no error is flagged
     * warnings maybe set
     * */
    bool AllOk();

    /**
     * TODO
     * allows mixing with other booleans
     * */
    operator bool();

    union {

        union {

               /**
                * The whole set of bits fits in a 32 bit unsigned
                */
               uint32 format_as_uint32;


               BitBoolean<uint32, 0u> functionError;


               /**
                * Initialization error
                */
               BitBoolean<uint32, 1u> initialisationError;


               /**
                * TODO
                * Error due to an operating system error
                */
               BitBoolean<uint32, 2u> OSError;

               /**
                * TODO
                * Parameters error
                * Specific to RPC applications: call parameters incompatibilities for the desired function (notPrototypeMismatch)
                */
               /**
                * Illegal operation
                */

               BitBoolean<uint32, 3u> illegalOperation;

               /**
                * Sharing error
                */
               BitBoolean<uint32, 4u> errorSharing;

               /**
                * Access denied
                */
               BitBoolean<uint32, 5u> accessDenied;

               /**
                * Exception
                */
               BitBoolean<uint32, 6u> exception;


               /**
                * Timeout occurred
                */
               BitBoolean<uint32, 7u> timeout;

               /**
                * Error during communication
                */
               BitBoolean<uint32, 8u> communicationError;

               /**
                * Syntax error
                */
               BitBoolean<uint32, 8u> syntaxError;

               bool notSyntaxError:1;

               /**
                * Specific to RPC applications: the desired function was not found (notMethodNotFound)
                * Unsupported feature
                */
               bool notUnsupportedFeature:1;

               /**
                * unexpected inconsistencies in the internal setup of a system/object
                */
               bool notInternalSetupError:1;

               /**
                * TODO
                * */
               int unmapped:19;

           };



        struct {
            /**
             * TODO
             * The return completed ok / not ok
             * meaning depends on actual function
             * */
            bool functionReturn:1;

            /**
             * Initialization error
             */
            bool notInitialisationError:1;

            /**
             * TODO
             * Error due to an operating system error
             */
            bool notOSError:1;
            /**
             * TODO
             * Parameters error
             * Specific to RPC applications: call parameters incompatibilities for the desired function (notPrototypeMismatch)
             */
            bool notParametersError:1;
            /**
             * Illegal operation
             */
            bool notIllegalOperation:1;
            /**
             * Sharing error
             */
            bool notErrorSharing:1;















            class DLL_API ReturnType{
            public:
                /**
                 * TODO
                 * if allOk is false then the error.functionReturn is set to false
                 * all other errors are always cleared (set to 1)
                 * */
                ReturnType(bool allOk=false);

                /**
                 * TODO
                 * is no error is flagged
                 * warnings maybe set
                 * */
                bool AllOk();

                /**
                 * TODO
                 * allows mixing with other booleans
                 * */
                operator bool();

                union {

                    union {

                           /**
                            * The whole set of bits fits in a 32 bit unsigned
                            */
                           uint32 format_as_uint32;


                           BitBoolean<uint32, 0u> functionError;


                           /**
                            * Initialization error
                            */
                           BitBoolean<uint32, 1u> initialisationError;


                           /**
                            * TODO
                            * Error due to an operating system error
                            */
                           BitBoolean<uint32, 2u> OSError;

                           /**
                            * TODO
                            * Parameters error
                            * Specific to RPC applications: call parameters incompatibilities for the desired function (notPrototypeMismatch)
                            */
                           /**
                            * Illegal operation
                            */

                           BitBoolean<uint32, 3u> illegalOperation;

                           /**
                            * Sharing error
                            */
                           BitBoolean<uint32, 4u> errorSharing;

                           /**
                            * Access denied
                            */
                           BitBoolean<uint32, 5u> accessDenied;

                           /**
                            * Exception
                            */
                           BitBoolean<uint32, 6u> exception;


                           /**
                            * Timeout occurred
                            */
                           BitBoolean<uint32, 7u> timeout;

                           /**
                            * Error during communication
                            */
                           BitBoolean<uint32, 8u> communicationError;

                           /**
                            * Syntax error
                            */
                           BitBoolean<uint32, 8u> syntaxError;

                           bool notSyntaxError:1;

                           /**
                            * Specific to RPC applications: the desired function was not found (notMethodNotFound)
                            * Unsupported feature
                            */
                           bool notUnsupportedFeature:1;

                           /**
                            * unexpected inconsistencies in the internal setup of a system/object
                            */
                           bool notInternalSetupError:1;

                           /**
                            * TODO
                            * */
                           int unmapped:19;

                       };



                    struct {
                        /**
                         * TODO
                         * The return completed ok / not ok
                         * meaning depends on actual function
                         * */
                        bool functionReturn:1;

                        /**
                         * Initialization error
                         */
                        bool notInitialisationError:1;

                        /**
                         * TODO
                         * Error due to an operating system error
                         */
                        bool notOSError:1;
                        /**
                         * TODO
                         * Parameters error
                         * Specific to RPC applications: call parameters incompatibilities for the desired function (notPrototypeMismatch)
                         */
                        bool notParametersError:1;
                        /**
                         * Illegal operation
                         */
                        bool notIllegalOperation:1;
                        /**
                         * Sharing error
                         */
                        bool notErrorSharing:1;
                        /**
                         * Access denied
                         */
                        bool notErrorAccessDenied:1;
                        /**
                         * Exception
                         */
                        bool notException:1;
                        /**
                         * Timeout occurred
                         */
                        bool notTimeout:1; //9
                        /**
                         * Error during communication
                         */
                        bool notCommunicationError:1;
                        /**
                         * Syntax error
                         */
                        bool notSyntaxError:1;

                        /**
                         * Specific to RPC applications: the desired function was not found (notMethodNotFound)
                         * Unsupported feature
                         */
                        bool notUnsupportedFeature:1;

                        /**
                         * unexpected inconsistencies in the internal setup of a system/object
                         */
                        bool notInternalSetupError:1;

                        /**
                         * TODO
                         * */
                        int unmapped:19;
                    } error;

                    /**
                     * TODO
                     * allows one operation access to all errors
                     * */
                    int32 flags;
                };

            };


            /*---------------------------------------------------------------------------*/
            /*                        Inline method definitions                          */
            /*---------------------------------------------------------------------------*/

            inline ReturnType::ReturnType(bool allOk){
                flags = -1;
                error.functionReturn = allOk;
            }

            inline bool ReturnType::AllOk(){
                return  (flags == -1);
            }

            inline ReturnType::operator bool(){
                return AllOk();
            }

            }
            /**
             * Access denied
             */
            bool notErrorAccessDenied:1;
            /**
             * Exception
             */
            bool notException:1;
            /**
             * Timeout occurred
             */
            bool notTimeout:1; //9
            /**
             * Error during communication
             */
            bool notCommunicationError:1;
            /**
             * Syntax error
             */
            bool notSyntaxError:1;

            /**
             * Specific to RPC applications: the desired function was not found (notMethodNotFound)
             * Unsupported feature
             */
            bool notUnsupportedFeature:1;

            /**
             * unexpected inconsistencies in the internal setup of a system/object
             */
            bool notInternalSetupError:1;

            /**
             * TODO
             * */
            int unmapped:19;
        } error;

        /**
         * TODO
         * allows one operation access to all errors
         * */
        int32 flags;
    };

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline ReturnType::ReturnType(bool allOk){
    flags = -1;
    error.functionReturn = allOk;
}

inline bool ReturnType::AllOk(){
    return  (flags == -1);
}

inline ReturnType::operator bool(){
    return AllOk();
}

}

#endif /* L0TYPES_RETURNTYPE_H_ */
	
